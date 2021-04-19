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

#include "generalizedMotionState.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::generalizedMotionState::generalizedMotionState()
:
    ubar_(),
    vbar_(),
    abar_()
{}

Foam::generalizedMotionState::generalizedMotionState
(
    const label nFreq
)
:
    ubar_(List<scalar>(nFreq, Zero)),
    vbar_(List<scalar>(nFreq, Zero)),
    abar_(List<scalar>(nFreq, Zero))
{}


Foam::generalizedMotionState::generalizedMotionState
(
    const dictionary& dict
)
{
    ITstream& is = dict.lookup("ubar");
    is >> static_cast<List<scalar>&>(ubar_);

    is = dict.lookup("vbar");
    is >> static_cast<List<scalar>&>(vbar_);

    is = dict.lookup("abar");
    is >> static_cast<List<scalar>&>(abar_);
}


Foam::generalizedMotionState::generalizedMotionState
(
    const generalizedMotionState& gMS
)
:
    ubar_(gMS.ubar()),
    vbar_(gMS.vbar()),
    abar_(gMS.abar())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::generalizedMotionState::~generalizedMotionState()
{}


// ************************************************************************* //
