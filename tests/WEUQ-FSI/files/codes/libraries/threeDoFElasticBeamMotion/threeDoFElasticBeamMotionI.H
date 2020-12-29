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

inline const Foam::fvMesh& Foam::threeDoFElasticBeamMotion::mesh() const
{
    return mesh_;
}

inline const Foam::Time& Foam::threeDoFElasticBeamMotion::runTime() const
{
    return runTime_;
}

inline const Foam::List<Foam::word>& Foam::threeDoFElasticBeamMotion::patches() const
{
    return patches_;
}

inline const Foam::scalar& Foam::threeDoFElasticBeamMotion::rhoInf() const
{
    return rhoInf_;
}

inline const Foam::point& Foam::threeDoFElasticBeamMotion::origin() const
{
    return origin_;
}

inline const Foam::vector& Foam::threeDoFElasticBeamMotion::direction() const
{
    return direction_;
}

inline const Foam::label& Foam::threeDoFElasticBeamMotion::nNode() const
{
    return nNode_;
}

inline const Foam::label& Foam::threeDoFElasticBeamMotion::nFreq() const
{
    return nFreq_;
}


inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::frequency() const
{
    return frequency_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::length() const
{
    return length_;
}

inline const Foam::List<Foam::vector>& Foam::threeDoFElasticBeamMotion::mass() const
{
    return mass_;
}

inline const Foam::List<Foam::List<Foam::vector> >& Foam::threeDoFElasticBeamMotion::modalShape() const
{
    return modalShape_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::m0() const
{
    return m0_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::k0() const
{
    return k0_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::c0() const
{
    return c0_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBeamMotion::zeta0() const
{
    return zeta0_;
}

inline const Foam::generalizedMotionState& Foam::threeDoFElasticBeamMotion::motionState() const
{
    return motionState_;
}

inline const Foam::generalizedMotionState& Foam::threeDoFElasticBeamMotion::motionStateOld() const
{
    return motionStateOld_;
}

inline Foam::generalizedMotionState& Foam::threeDoFElasticBeamMotion::motionState()
{
    return motionState_;
}

inline Foam::generalizedMotionState& Foam::threeDoFElasticBeamMotion::motionStateOld()
{
    return motionStateOld_;
}

// ************************************************************************* //
