/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2018 OpenFOAM Foundation
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
    Foam::functionObjects::threeDoFElasticBodyState

Description
    Writes the 3-DoF motion state.

    Example of function object specification:
    \verbatim
    threeDoFElasticBodyState
    {
        type           threeDoFElasticBodyState;
        libs           ("libelasticBodyDynamics.so");
    }
    \endverbatim

Usage
    \table
        Property     | Description                         | Required | Default value
        type         | type name: threeDoFElasticBodyState | yes      |
    \endtable

See also
    Foam::functionObjects::fvMeshFunctionObject
    Foam::functionObjects::logFiles

SourceFiles
    threeDoFElasticBodyState.C

\*---------------------------------------------------------------------------*/

#ifndef threeDoFElasticBodyState_H
#define threeDoFElasticBodyState_H

#include "fvMeshFunctionObject.H"
#include "logFiles.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                   Class threeDoFElasticBodyState Declaration
\*---------------------------------------------------------------------------*/

class threeDoFElasticBodyState
:
    public fvMeshFunctionObject,
    public logFiles
{

    // Private Member Functions

        //- Disallow default bitwise copy construct
        threeDoFElasticBodyState(const threeDoFElasticBodyState&);

        //- Disallow default bitwise assignment
        void operator=(const threeDoFElasticBodyState&);


protected:

    // Protected Member Functions

        //- overloaded writeFileHeader from writeFile
        virtual void writeFileHeader(const label i = 0);


public:

    //- Runtime type information
    TypeName("threeDoFElasticBodyState");


    // Constructors

        //- Construct from Time and dictionary
        threeDoFElasticBodyState
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~threeDoFElasticBodyState();


    // Member Functions

        //- Read the threeDoFElasticBodyState data
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Write the threeDoFElasticBodyState
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
