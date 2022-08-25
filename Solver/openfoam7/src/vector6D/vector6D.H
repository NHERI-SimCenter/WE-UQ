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
    Foam::vector6D

SourceFiles
    vector6DI.H
    vector6D.C

\*---------------------------------------------------------------------------*/

#ifndef vector6D_H
#define vector6D_H

#include "scalar.H"
#include "vector.H"
#include "tensor.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

class vector6D;
Istream& operator>>(Istream& is, vector6D&);
Ostream& operator<<(Ostream& os, const vector6D& C);


/*---------------------------------------------------------------------------*\
                           Class vector6D Declaration
\*---------------------------------------------------------------------------*/

class vector6D
{
    // private data

        //- First vector part of the vector6D
        vector v_;

        //- Second vector part of the vector6D
        vector w_;

public:

    //- Component type
    typedef vector6D cmptType;

    // Member constants

        //- Rank of vector6Dnion is 1
        static const direction rank = 1;

    // Static data members

        static const char* const typeName;

        static const vector6D zero;
        static const vector6D one;


    // Constructors

        //- Construct null
        inline vector6D();

        //- Construct given two vector parts
        inline vector6D(const vector& v, const vector& w);
        
        //- Construct as a copy
        inline vector6D(const vector6D& vw);

        //- Construct from Istream
        vector6D(Istream&);


    // Member functions

           // Access

               //- First vector part of the vector6D
               inline const vector& v() const;

               //- Second vector part of the vector6D
               inline const vector& w() const;


           // Edit

               //- First vector part of the vector6D
               inline vector& v();

               //- Second vector part of the vector6D
               inline vector& w();


    // Member operators

        inline void operator=(const vector6D&);
        inline void operator+=(const vector6D&);
        inline void operator-=(const vector6D&);

        inline void operator*=(const scalar);
        inline void operator/=(const scalar);


    // IOstream operators

        friend Istream& operator>>(Istream& is, vector6D&);
        friend Ostream& operator<<(Ostream& os, const vector6D& C);
};


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

//- Return a string representation of a vector6D
word name(const vector6D&);

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const vector6D& q1, const vector6D& q2);
inline bool operator!=(const vector6D& q1, const vector6D& q2);

inline vector6D operator+(const vector6D& q1, const vector6D& q2);
inline vector6D operator-(const vector6D& q);
inline vector6D operator-(const vector6D& q1, const vector6D& q2);
inline vector6D operator*(const scalar s, const vector6D& q);
inline vector6D operator*(const vector6D& q, const scalar s);
inline vector6D operator/(const vector6D& q, const scalar s);

inline scalar operator&(const vector6D& q1, const vector6D& q2);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "vector6DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
