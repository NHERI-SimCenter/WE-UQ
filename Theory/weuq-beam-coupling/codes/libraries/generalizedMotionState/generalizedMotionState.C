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
    u0_(),
    v0_(),
    a0_(),
    p0_()
{}

Foam::generalizedMotionState::generalizedMotionState
(
    const label nFreq
)
:
    u0_(List<scalar>(nFreq, Zero)),
    v0_(List<scalar>(nFreq, Zero)),
    a0_(List<scalar>(nFreq, Zero)),
    p0_(List<scalar>(nFreq, Zero))
{}


Foam::generalizedMotionState::generalizedMotionState
(
    const dictionary& dict
)
{
    ITstream& is = dict.lookup("u0");
    is >> static_cast<List<scalar>&>(u0_);

    is = dict.lookup("v0");
    is >> static_cast<List<scalar>&>(v0_);

    is = dict.lookup("a0");
    is >> static_cast<List<scalar>&>(a0_);

    is = dict.lookup("p0");
    is >> static_cast<List<scalar>&>(p0_);
}


Foam::generalizedMotionState::generalizedMotionState
(
    const generalizedMotionState& gMS
)
:
    u0_(gMS.u0()),
    v0_(gMS.v0()),
    a0_(gMS.a0()),
    p0_(gMS.p0())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::generalizedMotionState::~generalizedMotionState()
{}


// ************************************************************************* //
