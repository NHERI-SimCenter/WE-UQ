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
    Foam::generalizedMotion

Description
    three degree of freedom motion for a rigid body.

    Angular momentum stored in body fixed reference frame.  Reference
    orientation of the body (where Q = I) must align with the cartesian axes
    such that the Inertia tensor is in principle component form.  Can add
    restraints (e.g. a spring) and constraints (e.g. motion may only be on a
    plane).

    The time-integrator for the motion is run-time selectable with options for
    symplectic (explicit), Crank-Nicolson and Newmark schemes.

SourceFiles
    generalizedMotionI.H
    generalizedMotion.C
    generalizedMotionIO.C

\*---------------------------------------------------------------------------*/

#ifndef generalizedMotion_H
#define generalizedMotion_H

#include "generalizedMotionState.H"
#include "pointField.H"
#include "autoPtr.H"
#include "Switch.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declarations
class generalizedSolver;

/*---------------------------------------------------------------------------*\
                      Class generalizedMotion Declaration
\*---------------------------------------------------------------------------*/

class generalizedMotion
{
    friend class generalizedSolver;

    // Private data

        //- Motion state data object
        generalizedMotionState motionState_;

        //- Motion state data object for previous time-step
        generalizedMotionState motionState0_;

        //- Generalized mass of beam
        List<scalar> mbar_;

        //- Generalized stiffness of beam
        List<scalar> kbar_;

        //- Generalized damping of beam
        List<scalar> cbar_;

        //- Switch to turn reporting of motion data on and off
        Switch report_;

        //- Motion solver
        autoPtr<generalizedSolver> solver_;


    // Private Member Functions

        //- Update and relax accelerations from the force and torque
        void updateAcceleration(const List<scalar>& fbar);

        // Access

            //- Return access to generalized displacement
            inline const List<scalar>& ubar() const;

            //- Return access to generalized velocity
            inline const List<scalar>& vbar() const;

            //- Return access to generalized acceleration
            inline const List<scalar>& abar() const;

        // Edit

            //- Return access to generalized displacement
            inline List<scalar>& ubar();

            //- Return access to generalized velocity
            inline List<scalar>& vbar();

            //- Return access to generalized acceleration
            inline List<scalar>& abar();


public:

    // Constructors

        //- Construct null
        generalizedMotion();

        //- Construct from constant and state dictionaries
        generalizedMotion
        (
            const dictionary& dict,
            const dictionary& stateDict
        );

        //- Construct from constant, state dictionaries and mode number
        generalizedMotion
        (
            const dictionary& dict,
            const dictionary& stateDict,
            const label& nMode
        );

        //- Construct as copy
        generalizedMotion(const generalizedMotion&);


    //- Destructor
    virtual ~generalizedMotion();


    // Member Functions

        // Access

            //- Return access to generalized motion state
            inline const generalizedMotionState& state() const;

            //- Return access to generalized motion state
            inline const generalizedMotionState& state0() const;

            //- Return access to generalized mass
            inline const List<scalar>& mbar() const;

            //- Return access to generalized stiffness
            inline const List<scalar>& kbar() const;

            //- Return access to generalized damping
            inline const List<scalar>& cbar() const;

            //- Return the report Switch
            inline bool report() const;


        // Edit

            //- Store the motion state at the beginning of the time-step
            void newTime();

            //- Return access to generalized mass
            inline List<scalar>& mbar();

            //- Return access to generalized stiffness
            inline List<scalar>& kbar();

            //- Return access to generalized damping
            inline List<scalar>& cbar();


        // Update state

            //- Symplectic integration of velocities, orientation and position.
            //  Changes to Crank-Nicolson integration for subsequent iterations.
            virtual void solve
            (
                bool firstIter,
                const List<scalar>& fbar,
                scalar deltaT,
                scalar deltaT0
            );

            //- Report the status of the motion
            void status() const;


        //- Write
        virtual void write(Ostream&) const;

        //- Read coefficients dictionary and update system parameters,
        //  constraints and restraints but not the current state
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "generalizedMotionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
