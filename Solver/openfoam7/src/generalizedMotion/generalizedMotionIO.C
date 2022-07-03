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

#include "generalizedMotion.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::generalizedMotion::read(const dictionary& dict)
{
    dict.lookup("mbar") >> mbar_;
    dict.lookup("kbar") >> kbar_;
    dict.lookup("cbar") >> cbar_;

    report_ = dict.lookupOrDefault<Switch>("report", false);

    return true;
}


void Foam::generalizedMotion::write(Ostream& os) const
{
    motionState_.write(os);

    os.writeKeyword("mbar")
        << mbar_ << token::END_STATEMENT << nl;

    os.writeKeyword("kbar")
        << kbar_ << token::END_STATEMENT << nl;

    os.writeKeyword("cbar")
        << cbar_ << token::END_STATEMENT << nl;

    os.writeKeyword("report")
        << report_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
