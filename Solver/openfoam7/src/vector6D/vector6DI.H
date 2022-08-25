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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::vector6D::vector6D()
{}


inline Foam::vector6D::vector6D(const vector& v, const vector& w)
:
    v_(v),
    w_(w)
{}

inline Foam::vector6D::vector6D(const vector6D& vw)
:
    v_(vw.v()),
    w_(vw.w())
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::vector& Foam::vector6D::v() const
{
    return v_;
}


inline const Foam::vector& Foam::vector6D::w() const
{
    return w_;
}

inline Foam::vector& Foam::vector6D::v()
{
    return v_;
}

inline Foam::vector& Foam::vector6D::w()
{
    return w_;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void Foam::vector6D::operator=(const vector6D& q)
{
    w_ = q.w_;
    v_ = q.v_;
}

inline void Foam::vector6D::operator+=(const vector6D& q)
{
    w_ += q.w_;
    v_ += q.v_;
}

inline void Foam::vector6D::operator-=(const vector6D& q)
{
    w_ -= q.w_;
    v_ -= q.v_;
}

inline void Foam::vector6D::operator*=(const scalar s)
{
    w_ *= s;
    v_ *= s;
}

inline void Foam::vector6D::operator/=(const scalar s)
{
    w_ /= s;
    v_ /= s;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool Foam::operator==(const vector6D& q1, const vector6D& q2)
{
    return (equal(q1.v(), q2.v()) && equal(q1.w(), q2.w()));
}


inline bool Foam::operator!=(const vector6D& q1, const vector6D& q2)
{
    return !operator==(q1, q2);
}


inline Foam::vector6D Foam::operator+
(
    const vector6D& q1,
    const vector6D& q2
)
{
    return vector6D(q1.v() + q2.v(), q1.w() + q2.w());
}


inline Foam::vector6D Foam::operator-(const vector6D& q)
{
    return vector6D(-q.v(), -q.w());
}


inline Foam::vector6D Foam::operator-
(
    const vector6D& q1,
    const vector6D& q2
)
{
    return vector6D(q1.v() - q2.v(), q1.w() - q2.w());
}


inline Foam::scalar Foam::operator&(const vector6D& q1, const vector6D& q2)
{
    return (q1.v() & q2.v()) + (q1.w() & q2.w());
}


inline Foam::vector6D Foam::operator*(const scalar s, const vector6D& q)
{
    return vector6D(s*q.v(), s*q.w());
}


inline Foam::vector6D Foam::operator*(const vector6D& q, const scalar s)
{
    return vector6D(s*q.v(), s*q.w());
}


inline Foam::vector6D Foam::operator/(const vector6D& q, const scalar s)
{
    return vector6D(q.v()/s, q.w()/s);
}


// ************************************************************************* //
