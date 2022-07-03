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

#include "generalizedMotion.H"
#include "generalizedSolver.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::generalizedMotion::generalizedMotion()
:
    motionState_(),
    motionState0_(),
    mbar_(),
    kbar_(),
    cbar_(),
    report_(false),
    solver_(nullptr)
{}


Foam::generalizedMotion::generalizedMotion
(
    const dictionary& dict,
    const dictionary& stateDict
)
:
    motionState_(stateDict),
    motionState0_(stateDict),
    mbar_(dict.lookup("mbar")),
    kbar_(dict.lookup("kbar")),
    cbar_(dict.lookup("cbar")),
    report_(dict.lookupOrDefault<Switch>("report", false)),
    solver_(generalizedSolver::New(dict.subDict("solver"), *this))
{
}


Foam::generalizedMotion::generalizedMotion
(
    const dictionary& dict,
    const dictionary& stateDict,
    const label& nMode
)
:
    motionState_(stateDict),
    motionState0_(stateDict),
    mbar_(nMode, Zero),
    kbar_(nMode, Zero),
    cbar_(nMode, Zero),
    report_(dict.lookupOrDefault<Switch>("report", false)),
    solver_(generalizedSolver::New(dict.subDict("solver"), *this))
{
}


Foam::generalizedMotion::generalizedMotion
(
    const generalizedMotion& gM
)
:
    motionState_(gM.motionState_),
    motionState0_(gM.motionState0_),
    mbar_(gM.mbar_),
    kbar_(gM.kbar_),
    cbar_(gM.cbar_),
    report_(gM.report_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::generalizedMotion::~generalizedMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::generalizedMotion::newTime()
{
    motionState0_ = motionState_;
}


void Foam::generalizedMotion::updateAcceleration
(
    const List<scalar>& fbar
)
{
    abar() = (fbar-cbar()*vbar()-kbar()*ubar())/mbar();
}


void Foam::generalizedMotion::solve
(
    bool firstIter,
    const List<scalar>& fbar,
    scalar deltaT,
    scalar deltaT0
)
{
    if (Pstream::master())
    {
        solver_->solve(firstIter, fbar, deltaT, deltaT0); 

        if (report_)
        {
            Info<< "generalized motion" << nl
                << "    generalized load: " << fbar << nl;

            status();
        }
    }

    Pstream::scatter(motionState_);
}


void Foam::generalizedMotion::status() const
{
    Info<< "    generalized displacement: " << ubar() << nl
        << "    generalized velocity: " << vbar() << nl
        << "    generalized acceleration: " << abar()
        << endl;
}


// ************************************************************************* //
