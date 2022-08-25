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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

inline Foam::List<Foam::scalar>& Foam::generalizedSolver::ubar()
{
    return body_.motionState_.ubar();
}

inline Foam::List<Foam::scalar>& Foam::generalizedSolver::vbar()
{
    return body_.motionState_.vbar();
}

inline Foam::List<Foam::scalar>& Foam::generalizedSolver::abar()
{
    return body_.motionState_.abar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedSolver::ubar0() const
{
    return body_.motionState0_.ubar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedSolver::vbar0() const
{
    return body_.motionState0_.vbar();
}

inline const Foam::List<Foam::scalar>& Foam::generalizedSolver::abar0() const
{
    return body_.motionState0_.abar();
}

//- Update and relax accelerations from the force and torque
inline void Foam::generalizedSolver::updateAcceleration
(
    const List<scalar>& fbar
)
{
    body_.updateAcceleration(fbar);
}


// ************************************************************************* //
