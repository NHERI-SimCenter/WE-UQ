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


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline Foam::generalizedMotion& Foam::threeDoFElasticBodyMotion::gMotion()
{
    return gMotion_;
}

inline const Foam::point& Foam::threeDoFElasticBodyMotion::origin() const
{
    return origin_;
}

inline const Foam::vector& Foam::threeDoFElasticBodyMotion::direction() const
{
    return direction_;
}

inline const Foam::label& Foam::threeDoFElasticBodyMotion::nNode() const
{
    return nNode_;
}

inline const Foam::label& Foam::threeDoFElasticBodyMotion::nMode() const
{
    return nMode_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBodyMotion::frequency() const
{
    return frequency_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBodyMotion::dampingRatio() const
{
    return dampingRatio_;
}

inline const Foam::List<Foam::vector>& Foam::threeDoFElasticBodyMotion::mass() const
{
    return mass_;
}

inline const Foam::List<Foam::scalar>& Foam::threeDoFElasticBodyMotion::length() const
{
    return length_;
}

inline const Foam::List<Foam::List<Foam::vector> >& Foam::threeDoFElasticBodyMotion::mode() const
{
    return mode_;
}

inline const Foam::generalizedMotion& Foam::threeDoFElasticBodyMotion::gMotion() const
{
    return gMotion_;
}

inline const Foam::List<Foam::vector>& Foam::threeDoFElasticBodyMotion::u() const
{
    return u_;
}

inline const Foam::List<Foam::vector>& Foam::threeDoFElasticBodyMotion::v() const
{
    return v_;
}

inline const Foam::List<Foam::vector>& Foam::threeDoFElasticBodyMotion::a() const
{
    return a_;
}

inline Foam::point Foam::threeDoFElasticBodyMotion::transform
(
    const point& initialPoint
) const
{
    return
    (
        initialPoint
    );
}
// ************************************************************************* //
