/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2019 OpenFOAM Foundation
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

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::ubar() const
{
    return ubar_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::vbar() const
{
    return vbar_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotionState::abar() const
{
    return abar_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::ubar()
{
    return ubar_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::vbar()
{
    return vbar_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotionState::abar()
{
    return abar_;
}

// ************************************************************************* //
