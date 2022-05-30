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

#include "vector4D.H"
#include "IOstreams.H"
#include "OStringStream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const Foam::vector4D::typeName = "vector4D";
const Foam::vector4D Foam::vector4D::zero(vector::zero, 0);
const Foam::vector4D Foam::vector4D::one(vector::one, 1);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::vector4D::vector4D(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::word Foam::name(const vector4D& q)
{
    OStringStream buf;
    buf << '(' << q.v() << ',' << q.w() << ')';
    return buf.str();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>(Istream& is, vector4D& q)
{
    // Read beginning of vector4D
    is.readBegin("vector4D");

    is  >> q.v() >> q.w();

    // Read end of vector4D
    is.readEnd("vector4D");

    // Check state of Istream
    is.check("operator>>(Istream&, vector4D&)");

    return is;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const vector4D& q)
{
    os  << token::BEGIN_LIST
        << q.v() << token::SPACE << q.w()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
