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

Class
    Foam::vector4D

SourceFiles
    vector4DI.H
    vector4D.C

\*---------------------------------------------------------------------------*/

#ifndef vector4D_H
#define vector4D_H

#include "scalar.H"
#include "vector.H"
#include "tensor.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

class vector4D;
Istream& operator>>(Istream& is, vector4D&);
Ostream& operator<<(Ostream& os, const vector4D& C);


/*---------------------------------------------------------------------------*\
                           Class vector4D Declaration
\*---------------------------------------------------------------------------*/

class vector4D
{
    // private data

        //- Vector part of the vector4D
        vector v_;

        //- Scalar part of the vector4D
        scalar w_;

public:

    //- Component type
    typedef vector4D cmptType;

    // Member constants

        //- Rank of vector4Dnion is 1
        static const direction rank = 1;

    // Static data members

        static const char* const typeName;

        static const vector4D zero;
        static const vector4D one;


    // Constructors

        //- Construct null
        inline vector4D();

        //- Construct given scalar and vector parts
        inline vector4D(const vector& v, const scalar w);
        
        //- Construct as a copy
        inline vector4D(const vector4D& vw);

        //- Construct from Istream
        vector4D(Istream&);


    // Member functions

           // Access

               //- Vector part of the vector4D
               inline const vector& v() const;

               //- Scalar part of the vector4D
               inline const scalar& w() const;


           // Edit

               //- Vector part of the vector4D
               inline vector& v();

               //- Scalar part of the vector4D
               inline scalar& w();


    // Member operators

        inline void operator=(const vector4D&);
        inline void operator+=(const vector4D&);
        inline void operator-=(const vector4D&);

        inline void operator*=(const scalar);
        inline void operator/=(const scalar);


    // IOstream operators

        friend Istream& operator>>(Istream& is, vector4D&);
        friend Ostream& operator<<(Ostream& os, const vector4D& C);
};


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

//- Return a string representation of a vector4D
word name(const vector4D&);

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const vector4D& q1, const vector4D& q2);
inline bool operator!=(const vector4D& q1, const vector4D& q2);

inline vector4D operator+(const vector4D& q1, const vector4D& q2);
inline vector4D operator-(const vector4D& q);
inline vector4D operator-(const vector4D& q1, const vector4D& q2);
inline vector4D operator*(const scalar s, const vector4D& q);
inline vector4D operator*(const vector4D& q, const scalar s);
inline vector4D operator/(const vector4D& q, const scalar s);

inline scalar operator&(const vector4D& q1, const vector4D& q2);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "vector4DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
