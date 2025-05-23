/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fourDoFElasticBodyMotionSolver.H"
#include "addToRunTimeSelectionTable.H"
#include "polyMesh.H"
#include "pointPatchDist.H"
#include "pointConstraints.H"
#include "uniformDimensionedFields.H"
#include "binnedForces.H"
#include "mathematicalConstants.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(fourDoFElasticBodyMotionSolver, 0);

    addToRunTimeSelectionTable
    (
        motionSolver,
        fourDoFElasticBodyMotionSolver,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fourDoFElasticBodyMotionSolver::fourDoFElasticBodyMotionSolver
(
    const polyMesh& mesh,
    const IOdictionary& dict
)
:
    displacementMotionSolver(mesh, dict, typeName),
    motion_
    (
        coeffDict(),
        IOobject
        (
            "generalizedMotionState",
            mesh.time().timeName(),
            "uniform",
            mesh
        ).typeHeaderOk<IOdictionary>(true)
      ? IOdictionary
        (
            IOobject
            (
                "generalizedMotionState",
                mesh.time().timeName(),
                "uniform",
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::NO_WRITE,
                false
            )
        )
      : coeffDict()
    ),
    patches_(wordReList(coeffDict().lookup("patches"))),
    patchSet_(mesh.boundaryMesh().patchSet(patches_)),
    di_(readScalar(coeffDict().lookup("innerDistance"))),
    do_(readScalar(coeffDict().lookup("outerDistance"))),
    coupling_(coeffDict().lookupOrDefault<Switch>("coupling", true)),
    rhoInf_(1.0),
    rhoName_(coeffDict().lookupOrDefault<word>("rho", "rhoInf")),
    scale_
    (
        IOobject
        (
            "motionScale",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE,
            false
        ),
        pointMesh::New(mesh),
        dimensionedScalar("zero", dimless, 0.0)
    ),
    curTimeIndex_(-1)
{
    if (rhoName_ == "rhoInf")
    {
        rhoInf_ = readScalar(coeffDict().lookup("rhoInf"));
    }

    // Calculate scaling factor everywhere
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    {
        const pointMesh& pMesh = pointMesh::New(mesh);

        pointPatchDist pDist(pMesh, patchSet_, points0());
/*
        pointScalarField pDist
        (
            IOobject
            (
                "pointDistance",
                pMesh.db().time().timeName(),
                pMesh.db()
            ),
            pMesh,
            dimensionedScalar("y", dimLength, great)
        );

        const vector origin(motion_.origin());
        const vector dir(motion_.direction());

        forAll(points0(), pointi)
        {
            vector d = points0()[pointi]-origin;
            d = d - (d&dir) * dir;
            pDist[pointi] = mag(d);
        }  
*/
        // Scaling: 1 up to di then linear down to 0 at do away from patches
        scale_.primitiveFieldRef() =
            min
            (
                max
                (
                    (do_ - pDist.primitiveField())/(do_ - di_),
                    scalar(0)
                ),
                scalar(1)
            );

        // Convert the scale function to a cosine
        scale_.primitiveFieldRef() =
            min
            (
                max
                (
                    0.5
                  - 0.5
                   *cos(scale_.primitiveField()
                   *Foam::constant::mathematical::pi),
                    scalar(0)
                ),
                scalar(1)
            );

        pointConstraints::New(pMesh).constrain(scale_);
        scale_.write();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fourDoFElasticBodyMotionSolver::~fourDoFElasticBodyMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::fourDoFElasticBodyMotion&
Foam::fourDoFElasticBodyMotionSolver::motion() const
{
    return motion_;
}


Foam::tmp<Foam::pointField>
Foam::fourDoFElasticBodyMotionSolver::curPoints() const
{
    return points0() + pointDisplacement_.primitiveField();
}


void Foam::fourDoFElasticBodyMotionSolver::solve()
{
    const Time& t = mesh().time();

    if (mesh().nPoints() != points0().size())
    {
        FatalErrorInFunction
            << "The number of points in the mesh seems to have changed." << endl
            << "In constant/polyMesh there are " << points0().size()
            << " points; in the current mesh there are " << mesh().nPoints()
            << " points." << exit(FatalError);
    }

    // Store the motion state at the beginning of the time-stepbool
    bool firstIter = false;
    if (curTimeIndex_ != this->db().time().timeIndex())
    {
        motion_.gMotion().newTime();
        curTimeIndex_ = this->db().time().timeIndex();
        firstIter = true;
    }

    dictionary binData;    

    binData.add("direction", motion_.direction());
    binData.add("nBin", motion_.nNode()-1);
    binData.add("binMin", motion_.origin()&motion_.direction());
    binData.add("binDx", motion_.length());
    binData.add("cumulative", false);

    dictionary forcesDict;

    forcesDict.add("type", functionObjects::binnedForces::typeName);
    forcesDict.add("patches", patches_);
    forcesDict.add("rhoInf", rhoInf_);
    forcesDict.add("rho", rhoName_);
    forcesDict.add("CofR", motion_.origin());
    forcesDict.add("binData", binData);

    functionObjects::binnedForces f("forces", db(), forcesDict);

    f.calcForcesMoment();

    motion_.update
    (
        firstIter,
        f.binnedForceEff(),
        f.binnedMomentEff(),
        t.deltaTValue(),
        t.deltaT0Value()
    );

    if (coupling_)
    {
        // Update the displacements
        pointDisplacement_.primitiveFieldRef() =
        motion_.transform(points0(), scale_) - points0();

        // Displacement has changed. Update boundary conditions
        pointConstraints::New
        (
            pointDisplacement_.mesh()
        ).constrainDisplacement(pointDisplacement_);
    }
}


bool Foam::fourDoFElasticBodyMotionSolver::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp,
    const bool valid
) const
{
    IOdictionary dict
    (
        IOobject
        (
            "generalizedMotionState",
            mesh().time().timeName(),
            "uniform",
            mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    );

    motion_.gMotion().state().write(dict);
    return dict.regIOobject::write();
}


bool Foam::fourDoFElasticBodyMotionSolver::read()
{
    if (displacementMotionSolver::read())
    {
        motion_.read(coeffDict());
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
