/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2018 OpenFOAM Foundation
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

#include "fourDoFElasticBodyState.H"
#include "dynamicMotionSolverFvMesh.H"
#include "fourDoFElasticBodyMotionSolver.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(fourDoFElasticBodyState, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        fourDoFElasticBodyState,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::fourDoFElasticBodyState::fourDoFElasticBodyState
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    logFiles(obr_, name)
{
    read(dict);
    resetName(typeName);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::fourDoFElasticBodyState::~fourDoFElasticBodyState()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::fourDoFElasticBodyState::read(const dictionary& dict)
{
    fvMeshFunctionObject::read(dict);
    return true;
}


void Foam::functionObjects::fourDoFElasticBodyState::writeFileHeader(const label)
{
    OFstream& file = this->file();

    writeHeader(file, "Motion State");
    writeCommented(file, "Time");

    file<< tab
        << "generalized displacement" << tab
        << "generalized velocity" << tab
        << "generalized acceleration"<< endl;
}


bool Foam::functionObjects::fourDoFElasticBodyState::execute()
{
    return true;
}


bool Foam::functionObjects::fourDoFElasticBodyState::write()
{
    logFiles::write();

    if (Pstream::master())
    {
        const dynamicMotionSolverFvMesh& mesh =
            refCast<const dynamicMotionSolverFvMesh>(obr_);

        const fourDoFElasticBodyMotionSolver& motionSolver_ =
            refCast<const fourDoFElasticBodyMotionSolver>(mesh.motion());

        const fourDoFElasticBodyMotion& motion = motionSolver_.motion();

        writeTime(file());
        file()
            << tab
            << motion.gMotion().state().ubar() << tab
            << motion.gMotion().state().vbar() << tab
            << motion.gMotion().state().abar() << endl;
    }

    return true;
}


// ************************************************************************* //
