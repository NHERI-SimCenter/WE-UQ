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
    Foam::functionObjects::sixDoFElasticBodyState

Description
    Writes the 6-DoF motion state.

    Example of function object specification:
    \verbatim
    sixDoFElasticBodyState
    {
        type           sixDoFElasticBodyState;
        libs           ("libsixDoFElasticBodyState.so");
    }
    \endverbatim

Usage
    \table
        Property     | Description                       | Required | Default value
        type         | type name: sixDoFElasticBodyState | yes      |
    \endtable

See also
    Foam::functionObjects::fvMeshFunctionObject
    Foam::functionObjects::logFiles

SourceFiles
    sixDoFElasticBodyState.C

\*---------------------------------------------------------------------------*/

#ifndef sixDoFElasticBodyState_H
#define sixDoFElasticBodyState_H

#include "fvMeshFunctionObject.H"
#include "logFiles.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                   Class sixDoFElasticBodyState Declaration
\*---------------------------------------------------------------------------*/

class sixDoFElasticBodyState
:
    public fvMeshFunctionObject,
    public logFiles
{

    // Private Member Functions

        //- Disallow default bitwise copy construct
        sixDoFElasticBodyState(const sixDoFElasticBodyState&);

        //- Disallow default bitwise assignment
        void operator=(const sixDoFElasticBodyState&);


protected:

    // Protected Member Functions

        //- overloaded writeFileHeader from writeFile
        virtual void writeFileHeader(const label i = 0);


public:

    //- Runtime type information
    TypeName("sixDoFElasticBodyState");


    // Constructors

        //- Construct from Time and dictionary
        sixDoFElasticBodyState
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sixDoFElasticBodyState();


    // Member Functions

        //- Read the sixDoFElasticBodyState data
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Write the sixDoFElasticBodyState
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
