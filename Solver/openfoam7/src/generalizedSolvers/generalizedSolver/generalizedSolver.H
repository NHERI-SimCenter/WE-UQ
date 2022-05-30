/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2018 OpenFOAM Foundation
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
    Foam::generalizedSolver

Description

SourceFiles
    generalizedSolver.C
    newgeneralizedSolver.C

\*---------------------------------------------------------------------------*/

#ifndef generalizedSolver_H
#define generalizedSolver_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "generalizedMotion.H"
#include "runTimeSelectionTables.H"
#include "autoPtr.H"

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class generalizedSolver Declaration
\*---------------------------------------------------------------------------*/

class generalizedSolver
{
protected:

    // Protected data

        //- The rigid body
        generalizedMotion& body_;


    // Protected member functions

        //- Return non-const access to displacement
        inline List<scalar>& ubar();

        //- Return non-const access to velocity
        inline List<scalar>& vbar();

        //- Return non-const access to acceleration
        inline List<scalar>& abar();

        //- Return the displacement at previous time-step
        inline const List<scalar>& ubar0() const;

        //- Return the velocity at previous time-step
        inline const List<scalar>& vbar0() const;

        //- Return the acceleration at previous time-step
        inline const List<scalar>& abar0() const;

        //- Update and relax accelerations from the force and torque
        inline void updateAcceleration
        (
            const List<scalar>& fbar
        );


public:

    //- Runtime type information
    TypeName("generalizedSolver");


    // Declare runtime construction

        declareRunTimeSelectionTable
        (
            autoPtr,
            generalizedSolver,
            dictionary,
            (
                const dictionary& dict,
                generalizedMotion& body
            ),
            (dict, body)
        );


    // Constructors

        // Construct for given body
        generalizedSolver(generalizedMotion& body);


    //- Destructor
    virtual ~generalizedSolver();


    // Selectors

        static autoPtr<generalizedSolver> New
        (
            const dictionary& dict,
            generalizedMotion& body
        );


    // Member Functions

        //- Drag coefficient
        virtual void solve
        (
            bool firstIter,
            const List<scalar>& fbar,
            scalar deltaT,
            scalar deltaT0
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "generalizedSolverI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
