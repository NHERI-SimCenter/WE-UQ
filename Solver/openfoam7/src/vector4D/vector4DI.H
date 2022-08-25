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

inline Foam::vector4D::vector4D()
{}


inline Foam::vector4D::vector4D(const vector& v, const scalar w)
:
    v_(v),
    w_(w)
{}


inline Foam::vector4D::vector4D(const vector4D& vw)
:
    v_(vw.v()),
    w_(vw.w())
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const Foam::scalar& Foam::vector4D::w() const
{
    return w_;
}


inline const Foam::vector& Foam::vector4D::v() const
{
    return v_;
}


inline Foam::scalar& Foam::vector4D::w()
{
    return w_;
}


inline Foam::vector& Foam::vector4D::v()
{
    return v_;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void Foam::vector4D::operator=(const vector4D& q)
{
    w_ = q.w_;
    v_ = q.v_;
}

inline void Foam::vector4D::operator+=(const vector4D& q)
{
    w_ += q.w_;
    v_ += q.v_;
}

inline void Foam::vector4D::operator-=(const vector4D& q)
{
    w_ -= q.w_;
    v_ -= q.v_;
}

inline void Foam::vector4D::operator*=(const scalar s)
{
    w_ *= s;
    v_ *= s;
}

inline void Foam::vector4D::operator/=(const scalar s)
{
    w_ /= s;
    v_ /= s;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool Foam::operator==(const vector4D& q1, const vector4D& q2)
{
    return (equal(q1.v(), q2.v()) && equal(q1.w(), q2.w()));
}


inline bool Foam::operator!=(const vector4D& q1, const vector4D& q2)
{
    return !operator==(q1, q2);
}


inline Foam::vector4D Foam::operator+
(
    const vector4D& q1,
    const vector4D& q2
)
{
    return vector4D(q1.v() + q2.v(), q1.w() + q2.w());
}


inline Foam::vector4D Foam::operator-(const vector4D& q)
{
    return vector4D(-q.v(), -q.w());
}


inline Foam::vector4D Foam::operator-
(
    const vector4D& q1,
    const vector4D& q2
)
{
    return vector4D(q1.v() - q2.v(), q1.w() - q2.w());
}


inline Foam::scalar Foam::operator&(const vector4D& q1, const vector4D& q2)
{
    return q1.w()*q2.w() + (q1.v() & q2.v());
}


inline Foam::vector4D Foam::operator*(const scalar s, const vector4D& q)
{
    return vector4D(s*q.v(), s*q.w());
}


inline Foam::vector4D Foam::operator*(const vector4D& q, const scalar s)
{
    return vector4D(s*q.v(), s*q.w());
}


inline Foam::vector4D Foam::operator/(const vector4D& q, const scalar s)
{
    return vector4D(q.v()/s, q.w()/s);
}


// ************************************************************************* //
