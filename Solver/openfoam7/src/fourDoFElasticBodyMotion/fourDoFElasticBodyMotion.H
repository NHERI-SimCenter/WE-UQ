/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
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
    Foam::fourDoFElasticBodyMotion

Description
    Four degree of freedom motion for a elastic body.

    The time-integrator for the motion is run-time selectable with options for
    symplectic (explicit), Crank-Nicolson and Newmark schemes.

SourceFiles
    fourDoFElasticBodyMotionI.H
    fourDoFElasticBodyMotion.C
    fourDoFElasticBodyMotionIO.C

\*---------------------------------------------------------------------------*/

#ifndef fourDoFElasticBodygMotion_H
#define fourDoFElasticBodygMotion_H

#include "generalizedMotion.H"
#include "vector4D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class fourDoFElasticBodyMotion Declaration
\*---------------------------------------------------------------------------*/

class fourDoFElasticBodyMotion
:
    public generalizedMotion
{

    // Private data

        //- Origin of the beam
        point origin_;

        //- Direction of the beam
        vector direction_;

        //- Number of nodes on the beam
        label nNode_;

        //- Number of modes
        label nMode_;

        //- Frequency
        List<scalar> frequency_;

        //- Damping ratio
        List<scalar> dampingRatio_;

        //- Lumped mass of the beam
        List<vector4D> mass_;

        //- Segment length of the beam
        List<scalar> length_;

        //- Mode shape
        List<List<vector4D> > mode_;

        //- Motion state data object
        generalizedMotion gMotion_;

        //- Displacement
        List<vector4D> u_;

        //- Velocity
        List<vector4D> v_;

        //- Acceleration
        List<vector4D> a_;


    // Private Member Functions

        // Update generalized forces
        List<scalar> updateGeneralizedForces
        (
            const List<vector> binnedForces,
            const List<vector> binnedMoments
        );


public:

    // Constructors

        //- Construct null
        fourDoFElasticBodyMotion();

        //- Construct from constant and state dictionaries
        fourDoFElasticBodyMotion
        (
            const dictionary& dict,
            const dictionary& stateDict
        );

    //- Destructor
    virtual ~fourDoFElasticBodyMotion();


    // Member Functions

        // Access

            //- Return access to the origin of beam
            inline const point& origin() const;

            //- Return access to the direction of beam
            inline const vector& direction() const;

            //- Return access to the number of nodes
            inline const label& nNode() const;

            //- Return access to the number of modes
            inline const label& nMode() const;

            //- Return access to the frequency of each mode
            inline const List<scalar>& frequency() const;

            //- Return access to the damping ratio of each mode
            inline const List<scalar>& dampingRatio() const;

            //- Return access to lumped mass
            inline const List<vector4D>& mass() const;

            //- Return access to the length of each beam segment
            inline const List<scalar>& length() const;

            //- Return access to modal shapes
            inline const List<List<vector4D> >& mode() const;

            //- Return access to generalized motion
            inline const generalizedMotion& gMotion() const;

            //- Return access to displacement
            inline const List<vector4D>& u() const;

            //- Return access to velocity
            inline const List<vector4D>& v() const;

            //- Return access to acceleration
            inline const List<vector4D>& a() const;


        // Edit

            //- Return access to generalized motion
            inline generalizedMotion& gMotion();


        // Update state
        
            void update();

            void update
            (
                bool firstIter,
                const List<vector> forces,
                const List<vector> moments,
                scalar deltaT,
                scalar deltaT0
            );

        // Transformations

            //- Transform the given initial state point by the current motion
            //  state
            inline point transform(const point& initialPoints) const;

            //- Transform the given initial state pointField by the current
            //  motion state
            tmp<pointField> transform(const pointField& initialPoints) const;

            //- Transform the given initial state pointField by the current
            //  motion state scaled by the given scale
            tmp<pointField> transform
            (
                const pointField& initialPoints,
                const scalarField& scale
            ) const;


        //- Write
        void write(Ostream&) const;

        //- Read coefficients dictionary and update system parameters,
        //  constraints and restraints but not the current state
        bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fourDoFElasticBodyMotionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
