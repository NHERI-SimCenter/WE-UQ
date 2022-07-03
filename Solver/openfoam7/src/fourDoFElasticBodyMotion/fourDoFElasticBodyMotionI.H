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

inline Foam::generalizedMotion& Foam::fourDoFElasticBodyMotion::gMotion()
{
    return gMotion_;
}

inline const Foam::point& Foam::fourDoFElasticBodyMotion::origin() const
{
    return origin_;
}

inline const Foam::vector& Foam::fourDoFElasticBodyMotion::direction() const
{
    return direction_;
}

inline const Foam::label& Foam::fourDoFElasticBodyMotion::nNode() const
{
    return nNode_;
}

inline const Foam::label& Foam::fourDoFElasticBodyMotion::nMode() const
{
    return nMode_;
}

inline const Foam::List<Foam::scalar>& Foam::fourDoFElasticBodyMotion::frequency() const
{
    return frequency_;
}

inline const Foam::List<Foam::scalar>& Foam::fourDoFElasticBodyMotion::dampingRatio() const
{
    return dampingRatio_;
}

inline const Foam::List<Foam::vector4D>& Foam::fourDoFElasticBodyMotion::mass() const
{
    return mass_;
}

inline const Foam::List<Foam::scalar>& Foam::fourDoFElasticBodyMotion::length() const
{
    return length_;
}

inline const Foam::List<Foam::List<Foam::vector4D> >& Foam::fourDoFElasticBodyMotion::mode() const
{
    return mode_;
}

inline const Foam::generalizedMotion& Foam::fourDoFElasticBodyMotion::gMotion() const
{
    return gMotion_;
}

inline const Foam::List<Foam::vector4D>& Foam::fourDoFElasticBodyMotion::u() const
{
    return u_;
}

inline const Foam::List<Foam::vector4D>& Foam::fourDoFElasticBodyMotion::v() const
{
    return v_;
}

inline const Foam::List<Foam::vector4D>& Foam::fourDoFElasticBodyMotion::a() const
{
    return a_;
}

inline Foam::point Foam::fourDoFElasticBodyMotion::transform
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
