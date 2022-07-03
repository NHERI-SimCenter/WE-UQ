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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline Foam::List<Foam::scalar>& Foam::generalizedMotion::ubar()
{
    return motionState_.ubar();
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotion::vbar()
{
    return motionState_.vbar();
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotion::abar()
{
    return motionState_.abar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::ubar() const
{
    return motionState_.ubar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::vbar() const
{
    return motionState_.vbar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::abar() const
{
    return motionState_.abar();
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


inline Foam::List<Foam::scalar>& Foam::generalizedMotion::mbar()
{
    return mbar_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotion::kbar()
{
    return kbar_;
}

inline Foam::List<Foam::scalar>& Foam::generalizedMotion::cbar()
{
    return cbar_;
}

inline const Foam::generalizedMotionState& Foam::generalizedMotion::state() const
{
    return motionState_;
}

inline const Foam::generalizedMotionState& Foam::generalizedMotion::state0() const
{
    return motionState0_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::mbar() const
{
    return mbar_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::kbar() const
{
    return kbar_;
}

inline const Foam::List<Foam::scalar>& Foam::generalizedMotion::cbar() const
{
    return cbar_;
}

inline bool Foam::generalizedMotion::report() const
{
    return report_;
}

// ************************************************************************* //
