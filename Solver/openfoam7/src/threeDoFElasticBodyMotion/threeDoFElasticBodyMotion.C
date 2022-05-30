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

#include "threeDoFElasticBodyMotion.H"
#include "mathematicalConstants.H"
#include "interpolateXY.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::List<Foam::scalar> Foam::threeDoFElasticBodyMotion::updateGeneralizedForces
(
    const List<vector> forces,
    const List<vector> moments
)
{
    List<scalar> fbar(nMode(), Zero);
    List<vector> nodalForces(nNode(), Zero);

    forAll(nodalForces, indi)
    {
        if (indi == 0)
        {                        
            nodalForces[indi] = 0.5*forces[indi];
        }
        else if (indi == nNode()-1)
        {
            nodalForces[indi] = 0.5*forces[indi-1];
        }
        else
        {
            nodalForces[indi] = 0.5*(forces[indi]+forces[indi-1]);
        }
    }

    for (label indi = 0; indi < nMode(); indi++)
    {
        fbar[indi] = sum(nodalForces&mode()[indi]);
    }

    return fbar;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::threeDoFElasticBodyMotion::threeDoFElasticBodyMotion()
:
    origin_(Zero),
    direction_(Zero),
    nNode_(0),
    nMode_(0),
    frequency_(),
    dampingRatio_(),
    mass_(),
    length_(),
    gMotion_()
{}


Foam::threeDoFElasticBodyMotion::threeDoFElasticBodyMotion
(
    const dictionary& dict,
    const dictionary& stateDict
)
:
    origin_(dict.lookupOrDefault<vector>("origin", Zero)),
    direction_(dict.lookupOrDefault<vector>("direction", vector(0,0,1))),
    nNode_(dict.lookupOrDefault<label>("nNode", Zero)),
    nMode_(dict.lookupOrDefault<label>("nMode", Zero)),
    frequency_(dict.lookup("frequency")),
    dampingRatio_(dict.lookup("dampingRatio")),
    mass_(dict.lookup("mass")),
    length_(dict.lookup("length")),
    gMotion_(dict, stateDict, nMode_)
{
    mode_.setSize(nMode_);

    forAll(mode_, indi)
    {
        word imode(std::to_string(indi+1));
        word modeName("mode"+imode);

        ITstream& is = dict.lookup(modeName);
        is >> static_cast<List<vector>&>(mode_[indi]);

        gMotion_.mbar()[indi] = sum(mass_&mode_[indi]);
        gMotion_.kbar()[indi] = gMotion_.mbar()[indi]*sqr(constant::mathematical::twoPi*frequency_[indi]);
        gMotion_.cbar()[indi] = 2.0*constant::mathematical::twoPi*gMotion_.mbar()[indi]*dampingRatio_[indi]*frequency_[indi];
    }

    Info<< "generalized mass: " << gMotion_.mbar() << nl
        << "generalized stiffness: " << gMotion_.kbar() << nl
        << "generalized damping: " << gMotion_.cbar() << nl
        << "generalized displacement: " << gMotion_.state().ubar() << nl
        << "generalized velocity: " << gMotion_.state().vbar() << nl
        << "generalized acceleration: " << gMotion_.state().abar() << nl
        << endl;
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::threeDoFElasticBodyMotion::~threeDoFElasticBodyMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::threeDoFElasticBodyMotion::update()
{
    if (Pstream::master())
    {
        List<vector> u(nNode(), vector::zero);
        List<vector> v(nNode(), vector::zero);
        List<vector> a(nNode(), vector::zero);

        const List<scalar>& ubar(gMotion().state().ubar());
        const List<scalar>& vbar(gMotion().state().vbar());
        const List<scalar>& abar(gMotion().state().abar());

        forAll(mode(), indi)
        {
            u = u + mode()[indi]*ubar[indi];
            v = v + mode()[indi]*vbar[indi];
            a = a + mode()[indi]*abar[indi];
        }

        u_ = u;
        v_ = v;
        a_ = a;
    }

    Pstream::scatter(u_);
    Pstream::scatter(v_);
    Pstream::scatter(a_);
}

void Foam::threeDoFElasticBodyMotion::update
(
    bool firstIter,
    const List<vector> forces,
    const List<vector> moments,
    scalar deltaT,
    scalar deltaT0
)
{
    const List<scalar> fbar(updateGeneralizedForces(forces, moments));

    gMotion_.solve
    (
        firstIter,
        fbar,
        deltaT,
        deltaT0
    );

    update();
}


Foam::tmp<Foam::pointField> Foam::threeDoFElasticBodyMotion::transform
(
    const pointField& initialPoints,
    const scalarField& scale
) const
{
    tmp<pointField> tpoints(new pointField(initialPoints));
    pointField& points = tpoints.ref();

    scalarField dist0(nNode(), Zero);

    for (label indi = 1; indi < nNode(); indi++)
    {
        dist0[indi] = dist0[indi-1] + length()[indi-1];
    }

    const vectorField displacement0(u());

    const scalarField dist((initialPoints-origin())&direction());
    const vectorField displacement(interpolateXY(dist, dist0, displacement0));

    forAll(points, pointi)
    {
        // Move non-stationary points
        if (scale[pointi] > small)
        {
            // Use solid-body motion where scale = 1
            if (scale[pointi] > 1 - small)
            {
                points[pointi] = initialPoints[pointi]+displacement[pointi];
            }
            else
            {
                points[pointi] = initialPoints[pointi]+displacement[pointi]*scale[pointi];
            }
        }
    }

    return tpoints;
}


// ************************************************************************* //
