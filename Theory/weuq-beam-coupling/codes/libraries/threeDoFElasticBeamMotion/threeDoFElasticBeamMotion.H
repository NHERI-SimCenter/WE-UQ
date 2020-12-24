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
    Foam::threeDoFElasticBeamMotion

Description
    Holds the motion state of threeDoF object.  Wrapped up together
    to allow rapid scatter to other processors.  The processors must all
    maintain exactly the same state data to avoid any drift or inconsistency.

SourceFiles
    threeDoFElasticBeamMotionI.H
    threeDoFElasticBeamMotion.C
    threeDoFElasticBeamMotionIO.C

\*---------------------------------------------------------------------------*/

#ifndef threeDoFElasticBeamMotion_H
#define threeDoFElasticBeamMotion_H

#include "vector.H"
#include "point.H"
#include "dictionary.H"
#include "fvMesh.H"
#include "OFstream.H"
#include "generalizedMotionState.H"
#include "PtrList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                  Class threeDoFElasticBeamMotion Declaration
\*---------------------------------------------------------------------------*/

class threeDoFElasticBeamMotion
{
    // Private data

        const fvMesh& mesh_;

        const Time& runTime_;

        scalar t_;

        label curTimeIndex_;

        List<word> patches_;

        scalar rhoInf_;

        //- Origin of beam
        point origin_;

        //- Direction of beam
        vector direction_;

        //- Number of nodes on beam
        label nNode_;

        //- Number of modal shapes
        label nFreq_;

        //- Frequency
        List<scalar> frequency_;

        //- Segment length
        List<scalar> length_;

        //- Mass of beam
        List<vector> mass_;

        //- Modal shape
        List<List<vector> > modalShape_;


        //- Generalized mass of beam
        List<scalar> m0_;

        //- Generalized stiffness of beam
        List<scalar> k0_;

        //- Generalized damping of beam
        List<scalar> c0_;

        //- Generalized damping ratio of beam
        List<scalar> zeta0_;


        //- Motion state data object
        generalizedMotionState motionState_;

        //- Motion state data object for previous time-step
        generalizedMotionState motionStateOld_;

        //- File pointer
        PtrList<OFstream> filePtrs_;

public:

    // Constructors

        //- Construct from dictionary
        threeDoFElasticBeamMotion
        (
            const fvMesh&,
            const Time&,
            const dictionary&
        );

        //- Construct as copy
        threeDoFElasticBeamMotion(const threeDoFElasticBeamMotion&);


    //- Destructor
    ~threeDoFElasticBeamMotion();


    // Member Functions

        // Access

            //- Return access to mesh
            inline const fvMesh& mesh() const;

            //- Return access to time
            inline const Time& runTime() const;

            //- Return access to patch set
            inline const List<word>& patches() const;

            //- Return access to the origin of beam
            inline const scalar& rhoInf() const;

            //- Return access to the origin of beam
            inline const point& origin() const;

            //- Return access to the direction of beam
            inline const vector& direction() const;

            //- Return access to the number of nodes
            inline const label& nNode() const;

            //- Return access to the number of modal shapes
            inline const label& nFreq() const;

            //- Return access to frequency
            inline const List<scalar>& frequency() const;

            //- Return access to the coordinates of nodes
            inline const List<scalar>& length() const;

            //- Return access to mass
            inline const List<vector>& mass() const;

            //- Return access to modal shapes
            inline const List<List<vector> >& modalShape() const;


            //- Return access to generalized mass
            inline const List<scalar>& m0() const;

            //- Return access to generalized stiffness
            inline const List<scalar>& k0() const;

            //- Return access to generalized damping
            inline const List<scalar>& c0() const;

            //- Return access to generalized damping ratio
            inline const List<scalar>& zeta0() const;


            //- Return access to generalized motion state
            inline const generalizedMotionState& motionState() const;

            //- Return access to generalized motion state old
            inline const generalizedMotionState& motionStateOld() const;


        // Edit

            //- Return access to generalized motion state
            inline generalizedMotionState& motionState();

            //- Return access to generalized motion state old
            inline generalizedMotionState& motionStateOld();


            void newTime();

            void update();

            void calGeneralizedLoads();

            void createFiles();

            void write();

            OFstream& file();

            scalar integralAlongBeam(const List<scalar>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "threeDoFElasticBeamMotionI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
