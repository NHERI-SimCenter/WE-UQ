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

#include "threeDoFElasticBodyState.H"
#include "dynamicMotionSolverFvMesh.H"
#include "threeDoFElasticBodyMotionSolver.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(threeDoFElasticBodyState, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        threeDoFElasticBodyState,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::threeDoFElasticBodyState::threeDoFElasticBodyState
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

Foam::functionObjects::threeDoFElasticBodyState::~threeDoFElasticBodyState()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::threeDoFElasticBodyState::read(const dictionary& dict)
{
    fvMeshFunctionObject::read(dict);
    return true;
}


void Foam::functionObjects::threeDoFElasticBodyState::writeFileHeader(const label)
{
    OFstream& file = this->file();

    writeHeader(file, "Motion State");
    writeCommented(file, "Time");

    file<< tab
        << "generalized displacement" << tab
        << "generalized velocity" << tab
        << "generalized acceleration"<< endl;
}


bool Foam::functionObjects::threeDoFElasticBodyState::execute()
{
    return true;
}


bool Foam::functionObjects::threeDoFElasticBodyState::write()
{
    logFiles::write();

    if (Pstream::master())
    {
        const dynamicMotionSolverFvMesh& mesh =
            refCast<const dynamicMotionSolverFvMesh>(obr_);

        const threeDoFElasticBodyMotionSolver& motionSolver_ =
            refCast<const threeDoFElasticBodyMotionSolver>(mesh.motion());

        const threeDoFElasticBodyMotion& motion = motionSolver_.motion();

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
