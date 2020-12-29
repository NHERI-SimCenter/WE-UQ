/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "threeDoFElasticBeamMotion.H"
#include "mathematicalConstants.H"
#include "binforces.H"
#include "IFstream.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::threeDoFElasticBeamMotion::newTime()
{
    motionStateOld() = motionState();
}

void Foam::threeDoFElasticBeamMotion::update()
{
    if (curTimeIndex_ != runTime().timeIndex())
    {
        curTimeIndex_ = runTime().timeIndex();
        t_ = runTime().value();
        newTime();
    }

    calGeneralizedLoads();

    if (Pstream::master())
    {
        const scalar dt = runTime().deltaTValue();

        const scalar gamma = 0.5;
        const scalar beta = 0.25;

        List<scalar> k_tlide = k0() + gamma*c0()/beta/dt + m0()/beta/dt/dt;

        List<scalar> aCoeff = m0()/beta/dt + gamma*c0()/beta;
        List<scalar> bCoeff = m0()/beta/2.0 + dt*(gamma/beta/2.0-1.0)*c0();

        List<scalar> dp0(nFreq(), 0.0);
        List<scalar> du0(nFreq(), 0.0);
        List<scalar> dv0(nFreq(), 0.0);
        List<scalar> da0(nFreq(), 0.0);

        for (int indi=0; indi<nFreq(); indi++)
        {
            dp0[indi] = motionState().p0()[indi] - motionStateOld().p0()[indi];
            dp0[indi] = dp0[indi] + aCoeff[indi]*motionStateOld().v0()[indi] + bCoeff[indi]*motionStateOld().a0()[indi];

            du0[indi] = dp0[indi] / k_tlide[indi];
            dv0[indi] = gamma*du0[indi]/beta/dt - gamma*motionStateOld().v0()[indi]/beta + dt*(1.0-gamma/beta/2.0)*motionStateOld().a0()[indi];
            da0[indi] = du0[indi]/beta/dt/dt - motionStateOld().v0()[indi]/beta/dt - motionStateOld().a0()[indi]/beta/2.0;

            motionState().u0()[indi] = motionStateOld().u0()[indi] + du0[indi];
            motionState().v0()[indi] = motionStateOld().v0()[indi] + dv0[indi];
            motionState().a0()[indi] = motionStateOld().a0()[indi] + da0[indi];
        }
    }

    Pstream::scatter(motionState());
}

void Foam::threeDoFElasticBeamMotion::calGeneralizedLoads()
{
    dictionary dict;

    dict.add("patches", patches());
    dict.add("CofR", origin());

    dict.add("rho", "rhoInf");
    dict.add("rhoInf", rhoInf());

    dictionary binDict;

    binDict.add("nBin", nNode()-1);

    binDict.add("binMin", (origin()&direction()));
    binDict.add("binMax", (origin()&direction())+sum(length()));
    binDict.add("binDx", length());

    binDict.add("direction", direction());
    binDict.add("cumulative", false);

    dict.add("binData", binDict);

    binforces force("binforces", mesh(), mesh().thisDb(), dict);

    force.calcForcesMoment();

    List<vector> forceEff = force.forceEff();
    List<vector> momentEff = force.momentEff();

    List<vector> p(nNode(), vector::zero);

    forAll(p, indi)
    {
        if (indi == 0)
        {
            p[indi] = 0.5*vector(forceEff[indi].x(),forceEff[indi].y(),momentEff[indi].z());
        }
        else if (indi == nNode()-1)
        {
            p[indi] = 0.5*vector(forceEff[indi-1].x(),forceEff[indi-1].y(),momentEff[indi-1].z());
        }
        else
        {
            p[indi] = 0.5*vector(forceEff[indi-1].x(),forceEff[indi-1].y(),momentEff[indi-1].z())
                    + 0.5*vector(forceEff[indi].x(),forceEff[indi].y(),momentEff[indi].z());
        }
    }

    forAll(frequency_, indi)
    {
        motionState().p0()[indi] = sum(p&modalShape()[indi]); 
    }
}

Foam::scalar Foam::threeDoFElasticBeamMotion::integralAlongBeam
(
    const List<scalar>& value
)
{
    scalar intSum = 0.0;

    forAll(length(), indi)
    {
        intSum = intSum + 0.5*(value[indi]+value[indi+1])*length()[indi];
    }

    return intSum;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::threeDoFElasticBeamMotion::threeDoFElasticBeamMotion
(
    const fvMesh& mesh,
    const Time& runTime,
    const dictionary& dict
)
:
    mesh_(mesh),
    runTime_(runTime),
    t_(runTime.value()),
    curTimeIndex_(runTime.timeIndex()),
    rhoInf_(dict.lookupOrDefault<scalar>("rhoInf", 1.0)),
    origin_(dict.lookupOrDefault<vector>("origin", vector::zero)),
    direction_(dict.lookupOrDefault<vector>("direction", vector(0,0,1))),
    nNode_(dict.lookupOrDefault<label>("nNode", Zero)),
    nFreq_(dict.lookupOrDefault<label>("nFreq", Zero)),

    m0_(nFreq_, Zero),
    k0_(nFreq_, Zero),
    c0_(nFreq_, Zero),
    motionState_(nFreq_),
    motionStateOld_(nFreq_),
    filePtrs_()
{
    filePtrs_.clear();
    filePtrs_.setSize(1);

    if (Pstream::master())
    {
        createFiles();
    }

    Info<< "modal shape information:" << endl;

    ITstream& is = dict.lookup("patches");
    is >> static_cast<List<word>&>(patches_);
    Info<< "patches: " << patches_ << endl;

    is = dict.lookup("frequency");
    is >> static_cast<List<scalar>&>(frequency_);
    Info<< "frequency: " << frequency_ << endl;

    is = dict.lookup("length");
    is >> static_cast<List<scalar>&>(length_);
    Info<< "length: " << length_ << endl;

    is = dict.lookup("mass");
    is >> static_cast<List<vector>&>(mass_);
    Info<< "mass: " << mass_ << endl;

    is = dict.lookup("damping");
    is >> static_cast<List<scalar>&>(zeta0_);
    Info<< "damping ratio: " << zeta0_ << endl;

    modalShape_.setSize(nFreq_);

    forAll(modalShape_, indi)
    {
        word imodalShape(std::to_string(indi+1));
        word modalShapeName("modalShape"+imodalShape);

        is = dict.lookup(modalShapeName);
        is >> static_cast<List<vector>&>(modalShape_[indi]);

        Info<< modalShapeName << ": " << modalShape_[indi] << endl;

        m0_[indi] = sum(mass_&modalShape_[indi]);
        k0_[indi] = m0_[indi]*sqr(2.0*constant::mathematical::pi*frequency_[indi]);
        c0_[indi] = 2.0*constant::mathematical::twoPi*m0_[indi]*zeta0_[indi]*frequency_[indi];
    }

    Info<< "generalized mass: " << m0_ << endl;
    Info<< "generalized stiffness: " << k0_ << endl;
    Info<< "generalized damping: " << c0_ << endl;

    if (dict.found("motionState"))
    {
        const dictionary& subDict(dict.subDict("motionState"));
        motionState_ = generalizedMotionState(subDict);
    }

    calGeneralizedLoads();

    forAll(motionState().a0(), indi)
    {
        motionState().a0()[indi] = (motionState().p0()[indi]-c0()[indi]*motionState().v0()[indi]-k0()[indi]*motionState().u0()[indi])/m0()[indi];
    }

    Info<< motionState_ << endl;

    if (Pstream::master())
    {
        write();
    }
}


Foam::threeDoFElasticBeamMotion::threeDoFElasticBeamMotion
(
    const threeDoFElasticBeamMotion& tDoFEBM
)
:
    mesh_(tDoFEBM.mesh()),
    runTime_(tDoFEBM.runTime()),
    t_(tDoFEBM.t_),
    curTimeIndex_(tDoFEBM.curTimeIndex_),
    patches_(tDoFEBM.patches()),
    rhoInf_(tDoFEBM.rhoInf()),
    origin_(tDoFEBM.origin()),
    direction_(tDoFEBM.direction()),
    nNode_(tDoFEBM.nNode()),
    nFreq_(tDoFEBM.nFreq()),
    frequency_(tDoFEBM.frequency()),
    length_(tDoFEBM.length()),
    mass_(tDoFEBM.mass()),
    modalShape_(tDoFEBM.modalShape()),

    m0_(tDoFEBM.m0()),
    k0_(tDoFEBM.k0()),
    c0_(tDoFEBM.c0()),
    zeta0_(tDoFEBM.zeta0()),
    motionState_(tDoFEBM.motionState()),
    motionStateOld_(tDoFEBM.motionStateOld())
{}


void Foam::threeDoFElasticBeamMotion::createFiles()
{
    if (Pstream::master())
    {
        fileName baseDir = runTime().path();

        word outputPrefix = "postProcessing";
        word prefix = "threeDoFElasticBeamMotion";

        if (Pstream::parRun())
        {
            baseDir = baseDir/".."/outputPrefix;
        }
        else
        {
            baseDir = baseDir/outputPrefix;
        }

        const word startTimeName = runTime().timeName(runTime().startTime().value());

        fileName outputDir(baseDir/prefix/startTimeName);

        mkDir(outputDir);

        word fName_ = "generalizedMotionState";

        IFstream is(outputDir/(fName_ + ".dat"));

        if (is.good())
        {
            fName_ = fName_ + "_" + runTime().timeName();
        }

        filePtrs_.set(0, new OFstream(outputDir/(fName_ + ".dat")));
    }
}

Foam::OFstream& Foam::threeDoFElasticBeamMotion::file()
{
    if (!Pstream::master())
    {
        FatalErrorIn("Foam::OFstream& Foam::threeDoFElasticBeamMotion::file()")
            << "Request for file() can only be done by the master process"
            << abort(FatalError);
    }

    if (filePtrs_.size() != 1)
    {
        WarningIn("Foam::Ostream& Foam::threeDoFElasticBeamMotion::file()")
            << "Requested single file, but multiple files are present"
            << endl;
    }

    if (!filePtrs_.set(0))
    {
        FatalErrorIn("Foam::OFstream& Foam::threeDoFElasticBeamMotion::file()")
            << "File pointer at index " << 0 << " not allocated"
            << abort(FatalError);
    }

    return filePtrs_[0];
}

void Foam::threeDoFElasticBeamMotion::write()
{
    if (Pstream::master())
    {
        {
            file() << t_;

            for (label i = 0; i < nFreq(); i++)
            {
                file() << tab << motionState().u0()[i];
            }

            file() << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::threeDoFElasticBeamMotion::~threeDoFElasticBeamMotion()
{}


// ************************************************************************* //
