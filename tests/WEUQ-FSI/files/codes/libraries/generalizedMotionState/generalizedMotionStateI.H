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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::u0() const
{
    return u0_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::v0() const
{
    return v0_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::a0() const
{
    return a0_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::p0() const
{
    return p0_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::u0()
{
    return u0_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::v0()
{
    return v0_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::a0()
{
    return a0_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::p0()
{
    return p0_;
}

// ************************************************************************* //
