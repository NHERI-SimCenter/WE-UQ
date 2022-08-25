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

Class
    Foam::generalizedMotionState

Description
    Holds the motion state of threeDoF object.  Wrapped up together
    to allow rapid scatter to other processors.  The processors must all
    maintain exactly the same state data to avoid any drift or inconsistency.

SourceFiles
    generalizedMotionStateI.H
    generalizedMotionState.C
    generalizedMotionStateIO.C

\*---------------------------------------------------------------------------*/

#ifndef generalizedMotionState_H
#define generalizedMotionState_H

#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators
class generalizedMotionState;
Istream& operator>>(Istream&, generalizedMotionState&);
Ostream& operator<<(Ostream&, const generalizedMotionState&);


/*---------------------------------------------------------------------------*\
                  Class generalizedMotionState Declaration
\*---------------------------------------------------------------------------*/

class generalizedMotionState
{
    // Private data

        //- Generalized displacement of beam
        List<scalar> ubar_;

        //- Generalized velocity of beam
        List<scalar> vbar_;

        //- Generalized acceleration of beam
        List<scalar> abar_;

public:

    // Constructors

        //- Construct null
        generalizedMotionState();

        generalizedMotionState(const label);

        //- Construct from dictionary
        generalizedMotionState(const dictionary&);


    //- Destructor
    ~generalizedMotionState();


    // Member Functions

        // Access

            //- Return access to generalized displacement
            inline const List<scalar>& ubar() const;

            //- Return access to generalized velocity
            inline const List<scalar>& vbar() const;

            //- Return access to generalized acceleration
            inline const List<scalar>& abar() const;


        // Edit

            //- Return non-const access to generalized displacement
            inline List<scalar>& ubar();

            //- Return non-const access to generalized velocity
            inline List<scalar>& vbar();

            //- Return non-const access to generalized acceleration
            inline List<scalar>& abar();

        //- Write to dictionary
        void write(dictionary& dict) const;

        //- Write to stream
        void write(Ostream&) const;


    // IOstream Operators

        friend Istream& operator>>(Istream&, generalizedMotionState&);
        friend Ostream& operator<<(Ostream&, const generalizedMotionState&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "generalizedMotionStateI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
