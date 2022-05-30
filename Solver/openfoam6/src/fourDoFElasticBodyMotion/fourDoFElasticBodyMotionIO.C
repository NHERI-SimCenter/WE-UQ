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

\*---------------------------------------------------------------------------*/

#include "fourDoFElasticBodyMotion.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::fourDoFElasticBodyMotion::read(const dictionary& dict)
{
    dict.lookup("origin") >> origin_;
    dict.lookup("direction") >> direction_;

    dict.lookup("nNode") >> nNode_;
    dict.lookup("nMode") >> nMode_;

    dict.lookup("frequency") >> frequency_;
    dict.lookup("dampingRatio") >> dampingRatio_;

    dict.lookup("mass") >> mass_;
    dict.lookup("length") >> length_;

    return true;
}


void Foam::fourDoFElasticBodyMotion::write(Ostream& os) const
{
    gMotion_.write(os);

    os.writeKeyword("origin")
        << origin_ << token::END_STATEMENT << nl;

    os.writeKeyword("direction")
        << direction_ << token::END_STATEMENT << nl;

    os.writeKeyword("nNode")
        << nNode_ << token::END_STATEMENT << nl;

    os.writeKeyword("nMode")
        << nMode_ << token::END_STATEMENT << nl;

    os.writeKeyword("frequency")
        << frequency_ << token::END_STATEMENT << nl;

    os.writeKeyword("dampingRatio")
        << dampingRatio_ << token::END_STATEMENT << nl;

    os.writeKeyword("mass")
        << mass_ << token::END_STATEMENT << nl;

    os.writeKeyword("length")
        << length_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
