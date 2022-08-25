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

\*---------------------------------------------------------------------------*/

#include "generalizedMotionState.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::generalizedMotionState::write(dictionary& dict) const
{
    dict.add("ubar", ubar_);
    dict.add("vbar", vbar_);
    dict.add("abar", abar_);
}


void Foam::generalizedMotionState::write(Ostream& os) const
{
    os.writeKeyword("ubar")
        << vbar_ << token::END_STATEMENT << nl;
    os.writeKeyword("vbar")
        << vbar_ << token::END_STATEMENT << nl;
    os.writeKeyword("abar")
        << abar_ << token::END_STATEMENT << nl;

}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>
(
    Istream& is, generalizedMotionState& gMS
)
{
    is  >> gMS.ubar_
        >> gMS.vbar_
        >> gMS.abar_;

    // Check state of Istream
    is.check
    (
        "Foam::Istream& Foam::operator>>"
        "(Foam::Istream&, Foam::generalizedMotionState&)"
    );

    return is;
}


Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const generalizedMotionState& gMS
)
{
    os  << token::SPACE  << gMS.ubar()
        << token::SPACE  << gMS.vbar()
        << token::SPACE  << gMS.abar();

    // Check state of Ostream
    os.check
    (
        "Foam::Ostream& Foam::operator<<(Foam::Ostream&, "
        "const Foam::generalizedMotionState&)"
    );

    return os;
}


// ************************************************************************* //
