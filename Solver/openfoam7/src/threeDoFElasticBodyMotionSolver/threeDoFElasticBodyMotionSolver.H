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
    Foam::threeDoFElasticBodyMotionSolver

Description
    3-DoF elastic-body mesh motion solver for an fvMesh.

SourceFiles
    threeDoFElasticBodyMotionSolver.C

\*---------------------------------------------------------------------------*/

#ifndef threeDoFElasticBodyMotionSolver_H
#define threeDoFElasticBodyMotionSolver_H

#include "displacementMotionSolver.H"
#include "threeDoFElasticBodyMotion.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
        Class threeDoFElasticBodyMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class threeDoFElasticBodyMotionSolver
:
    public displacementMotionSolver
{
    // Private data

        //- Three DoF motion object
        threeDoFElasticBodyMotion motion_;

        wordReList patches_;

        //- Patches to integrate forces
        const labelHashSet patchSet_;

        //- Inner morphing distance (limit of solid-body region)
        const scalar di_;

        //- Outer morphing distance (limit of linear interpolation region)
        const scalar do_;

        //- Switch for test-mode in which only the
        //  gravitational body-force is applied
        Switch coupling_;

        //- Reference density required by the forces object for
        //  incompressible calculations, required if rho == rhoInf
        scalar rhoInf_;

        //- Name of density field, optional unless used for an
        //  incompressible simulation, when this needs to be specified
        //  as rhoInf
        word rhoName_;

        //- Current interpolation scale (1 at patches, 0 at distance_)
        pointScalarField scale_;

        //- Current time index (used for updating)
        label curTimeIndex_;


public:

    //- Runtime type information
    TypeName("threeDoFElasticBodyMotion");


    // Constructors

        //- Construct from polyMesh and IOdictionary
        threeDoFElasticBodyMotionSolver
        (
            const polyMesh&,
            const dictionary& dict
        );
        
        //- Disallow default bitwise copy construct
        threeDoFElasticBodyMotionSolver
        (
            const threeDoFElasticBodyMotionSolver&
        );


    //- Destructor
    ~threeDoFElasticBodyMotionSolver();


    // Member Functions

        //- Return the three DoF motion object
        const threeDoFElasticBodyMotion& motion() const;

        //- Return point location obtained from the current motion field
        virtual tmp<pointField> curPoints() const;

        //- Solve for motion
        virtual void solve();

        //- Write motion state information for restart
        virtual bool write() const;


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const threeDoFElasticBodyMotionSolver&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
