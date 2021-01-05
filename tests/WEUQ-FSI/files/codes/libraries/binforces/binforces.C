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

#include "binforces.H"
#include "fvcGrad.H"
#include "porosityModel.H"
#include "turbulentTransportModel.H"
#include "turbulentFluidThermoModel.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::binforces::initialise()
{
    if (initialised_)
    {
        return;
    }

    if (directForceDensity_)
    {
        if (!obr_.foundObject<volVectorField>(fDName_))
        {
            FatalErrorInFunction
                << "Could not find " << fDName_ << " in database."
                << exit(FatalError);
        }
    }
    else
    {
        if
        (
            !obr_.foundObject<volVectorField>(UName_)
         || !obr_.foundObject<volScalarField>(pName_)

        )
        {
            FatalErrorInFunction
                << "Could not find " << UName_ << ", " << pName_
                << exit(FatalError);
        }

        if
        (
            rhoName_ != "rhoInf"
         && !obr_.foundObject<volScalarField>(rhoName_)
        )
        {
            FatalErrorInFunction
                << "Could not find " << rhoName_
                << exit(FatalError);
        }
    }

    initialised_ = true;
}


Foam::tmp<Foam::volSymmTensorField>
Foam::binforces::devRhoReff() const
{
    typedef compressible::turbulenceModel cmpTurbModel;
    typedef incompressible::turbulenceModel icoTurbModel;

    if (obr_.foundObject<cmpTurbModel>(cmpTurbModel::propertiesName))
    {
        const cmpTurbModel& turb =
            obr_.lookupObject<cmpTurbModel>(cmpTurbModel::propertiesName);

        return turb.devRhoReff();
    }
    else if (obr_.foundObject<icoTurbModel>(icoTurbModel::propertiesName))
    {
        const incompressible::turbulenceModel& turb =
            obr_.lookupObject<icoTurbModel>(icoTurbModel::propertiesName);

        return rho()*turb.devReff();
    }
    else if (obr_.foundObject<fluidThermo>(fluidThermo::dictName))
    {
        const fluidThermo& thermo =
            obr_.lookupObject<fluidThermo>(fluidThermo::dictName);

        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);

        return -thermo.mu()*dev(twoSymm(fvc::grad(U)));
    }
    else if
    (
        obr_.foundObject<transportModel>("transportProperties")
    )
    {
        const transportModel& laminarT =
            obr_.lookupObject<transportModel>("transportProperties");

        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);

        return -rho()*laminarT.nu()*dev(twoSymm(fvc::grad(U)));
    }
    else if (obr_.foundObject<dictionary>("transportProperties"))
    {
        const dictionary& transportProperties =
             obr_.lookupObject<dictionary>("transportProperties");

        dimensionedScalar nu
        (
            "nu",
            dimViscosity,
            transportProperties.lookup("nu")
        );

        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);

        return -rho()*nu*dev(twoSymm(fvc::grad(U)));
    }
    else
    {
        FatalErrorInFunction
            << "No valid model for viscous stress calculation"
            << exit(FatalError);

        return volSymmTensorField::null();
    }
}


Foam::tmp<Foam::volScalarField> Foam::binforces::mu() const
{
    if (obr_.foundObject<fluidThermo>(basicThermo::dictName))
    {
        const fluidThermo& thermo =
             obr_.lookupObject<fluidThermo>(basicThermo::dictName);

        return thermo.mu();
    }
    else if
    (
        obr_.foundObject<transportModel>("transportProperties")
    )
    {
        const transportModel& laminarT =
            obr_.lookupObject<transportModel>("transportProperties");

        return rho()*laminarT.nu();
    }
    else if (obr_.foundObject<dictionary>("transportProperties"))
    {
        const dictionary& transportProperties =
             obr_.lookupObject<dictionary>("transportProperties");

        dimensionedScalar nu
        (
            "nu",
            dimViscosity,
            transportProperties.lookup("nu")
        );

        return rho()*nu;
    }
    else
    {
        FatalErrorInFunction
            << "No valid model for dynamic viscosity calculation"
            << exit(FatalError);

        return volScalarField::null();
    }
}


Foam::tmp<Foam::volScalarField> Foam::binforces::rho() const
{
    if (rhoName_ == "rhoInf")
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    "rho",
                    mesh_.time().timeName(),
                    mesh_
                ),
                mesh_,
                dimensionedScalar("rho", dimDensity, rhoRef_)
            )
        );
    }
    else
    {
        return(obr_.lookupObject<volScalarField>(rhoName_));
    }
}


Foam::scalar Foam::binforces::rho(const volScalarField& p) const
{
    if (p.dimensions() == dimPressure)
    {
        return 1.0;
    }
    else
    {
        if (rhoName_ != "rhoInf")
        {
            FatalErrorInFunction
                << "Dynamic pressure is expected but kinematic is provided."
                << exit(FatalError);
        }

        return rhoRef_;
    }
}


void Foam::binforces::applyBins
(
    const vectorField& Md,
    const vectorField& fN,
    const vectorField& fT,
    const vectorField& fP,
    const vectorField& d
)
{
    if (nBin_ == 1)
    {
        force_[0][0] += sum(fN);
        force_[1][0] += sum(fT);
        force_[2][0] += sum(fP);
        moment_[0][0] += sum(Md^fN);
        moment_[1][0] += sum(Md^fT);
        moment_[2][0] += sum(Md^fP);
    }
    else
    {
        scalarField dd((d & binDir_) - binMin_);

        forAll(dd, i)
        {
            label bini = 0;

            for (label ii = 0; ii < nBin_; ii++)
            {
                if (dd[i] >= (binPoints_[ii]&binDir_) && dd[i] < (binPoints_[ii+1]&binDir_))
                {
                    bini = ii;
                    break;
                }
            }

            force_[0][bini] += fN[i];
            force_[1][bini] += fT[i];
            force_[2][bini] += fP[i];
            moment_[0][bini] += (Md[i]-(Md[i]&binDir_)*binDir_)^fN[i];
            moment_[1][bini] += (Md[i]-(Md[i]&binDir_)*binDir_)^fT[i];
            moment_[2][bini] += (Md[i]-(Md[i]&binDir_)*binDir_)^fP[i];
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


Foam::binforces::binforces
(
    const word& name,
    const fvMesh& mesh,
    const objectRegistry& obr,
    const dictionary& dict
)
:
    mesh_(mesh),
    obr_(obr),
    force_(3),
    moment_(3),
    patchSet_(),
    pName_(word::null),
    UName_(word::null),
    rhoName_(word::null),
    directForceDensity_(false),
    fDName_(""),
    rhoRef_(vGreat),
    pRef_(0),
    coordSys_(),
    localSystem_(false),
    porosity_(false),
    nBin_(1),
    binDir_(Zero),
    binDx_(),
    binMin_(great),
    binMax_(-great),
    binPoints_(),
    binCumulative_(true),
    initialised_(false)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::binforces::~binforces()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::binforces::read(const dictionary& dict)
{
    initialised_ = false;

    directForceDensity_ = dict.lookupOrDefault("directForceDensity", false);

    const polyBoundaryMesh& pbm = mesh_.boundaryMesh();

    patchSet_ = pbm.patchSet(wordReList(dict.lookup("patches")));

    if (directForceDensity_)
    {
        // Optional entry for fDName
        fDName_ = dict.lookupOrDefault<word>("fD", "fD");
    }
    else
    {
        // Optional entries U and p
        pName_ = dict.lookupOrDefault<word>("p", "p");
        UName_ = dict.lookupOrDefault<word>("U", "U");
        rhoName_ = dict.lookupOrDefault<word>("rho", "rho");

        // Reference density needed for incompressible calculations
        if (rhoName_ == "rhoInf")
        {
            dict.lookup("rhoInf") >> rhoRef_;
        }

        // Reference pressure, 0 by default
        pRef_ = dict.lookupOrDefault<scalar>("pRef", 0.0);
    }

    coordSys_.clear();

    // Centre of rotation for moment calculations
    // specified directly, from coordinate system, or implicitly (0 0 0)
    if (!dict.readIfPresent<point>("CofR", coordSys_.origin()))
    {
        coordSys_ = coordinateSystem(obr_, dict);
        localSystem_ = true;
    }

    dict.readIfPresent("porosity", porosity_);

    if (dict.found("binData"))
    {
        const dictionary& binDict(dict.subDict("binData"));
        binDict.lookup("nBin") >> nBin_;

        if (nBin_ < 0)
        {
            FatalIOErrorInFunction(dict)
                << "Number of bins (nBin) must be zero or greater"
                << exit(FatalIOError);
        }
        else if ((nBin_ == 0) || (nBin_ == 1))
        {
            nBin_ = 1;
            forAll(force_, i)
            {
                force_[i].setSize(1);
                moment_[i].setSize(1);
            }
        }

        if (nBin_ > 1)
        {
            binDict.lookup("direction") >> binDir_;
            binDir_ /= mag(binDir_);

            if (binDict.found("binMin"))
            {
                binDict.lookup("binMin") >> binMin_;
            }
            else
            {
                forAllConstIter(labelHashSet, patchSet_, iter)
                {
                    label patchi = iter.key();
                    const polyPatch& pp = pbm[patchi];
                    scalarField d(pp.faceCentres() & binDir_);
                    binMin_ = min(min(d), binMin_);
                }
                reduce(binMin_, minOp<scalar>());
            }

            if (binDict.found("binMax"))
            {
                binDict.lookup("binMax") >> binMax_;
            }
            else
            {
                forAllConstIter(labelHashSet, patchSet_, iter)
                {
                    label patchi = iter.key();
                    const polyPatch& pp = pbm[patchi];
                    scalarField d(pp.faceCentres() & binDir_);
                    binMax_ = max(max(d), binMax_);
                }
                reduce(binMax_, maxOp<scalar>());
            }

            // slightly boost binMax so that region of interest is fully
            // within bounds
            binMax_ = 1.0001*(binMax_ - binMin_) + binMin_;

            if (binDict.found("binDx"))
            {
                binDict.lookup("binDx") >> binDx_;
            }
            else
            {
                binDx_ = List<scalar>(nBin_, 1.0);
                binDx_ = (binMax_ - binMin_)/scalar(nBin_)*binDx_;
            }

            // create the bin points used for writing
            binPoints_.setSize(nBin_+1);

            forAll(binPoints_, i)
            {
                if (i == 0)
                {
                    binPoints_[i] = binMin_*binDir_;
                }
                else
                {
                    binPoints_[i] = binPoints_[i-1]+binDir_*binDx_[i-1];
                }
            }

            binDict.lookup("cumulative") >> binCumulative_;

            // allocate storage for forces and moments
            forAll(force_, i)
            {
                force_[i].setSize(nBin_);
                moment_[i].setSize(nBin_);
            }
        }
    }

    if (nBin_ == 1)
    {
        // allocate storage for forces and moments
        force_[0].setSize(1);
        force_[1].setSize(1);
        force_[2].setSize(1);
        moment_[0].setSize(1);
        moment_[1].setSize(1);
        moment_[2].setSize(1);
    }

    return true;
}


void Foam::binforces::calcForcesMoment()
{
    initialise();

    force_[0] = Zero;
    force_[1] = Zero;
    force_[2] = Zero;

    moment_[0] = Zero;
    moment_[1] = Zero;
    moment_[2] = Zero;

    if (directForceDensity_)
    {
        const volVectorField& fD = obr_.lookupObject<volVectorField>(fDName_);

        const surfaceVectorField::Boundary& Sfb =
            mesh_.Sf().boundaryField();

        forAllConstIter(labelHashSet, patchSet_, iter)
        {
            label patchi = iter.key();

            vectorField Md
            (
                mesh_.C().boundaryField()[patchi] - coordSys_.origin()
            );

            scalarField sA(mag(Sfb[patchi]));

            // Normal force = surfaceUnitNormal*(surfaceNormal & forceDensity)
            vectorField fN
            (
                Sfb[patchi]/sA
               *(
                    Sfb[patchi] & fD.boundaryField()[patchi]
                )
            );

            // Tangential force (total force minus normal fN)
            vectorField fT(sA*fD.boundaryField()[patchi] - fN);

            //- Porous force
            vectorField fP(Md.size(), Zero);

            applyBins(Md, fN, fT, fP, mesh_.C().boundaryField()[patchi]);
        }
    }
    else
    {
        const volScalarField& p = obr_.lookupObject<volScalarField>(pName_);

        const surfaceVectorField::Boundary& Sfb =
            mesh_.Sf().boundaryField();

        tmp<volSymmTensorField> tdevRhoReff = devRhoReff();
        const volSymmTensorField::Boundary& devRhoReffb
            = tdevRhoReff().boundaryField();

        // Scale pRef by density for incompressible simulations
        scalar pRef = pRef_/rho(p);

        forAllConstIter(labelHashSet, patchSet_, iter)
        {
            label patchi = iter.key();

            vectorField Md
            (
                mesh_.C().boundaryField()[patchi] - coordSys_.origin()
            );

            vectorField fN
            (
                rho(p)*Sfb[patchi]*(p.boundaryField()[patchi] - pRef)
            );

            vectorField fT(Sfb[patchi] & devRhoReffb[patchi]);

            vectorField fP(Md.size(), Zero);

            applyBins(Md, fN, fT, fP, mesh_.C().boundaryField()[patchi]);
        }
    }

    if (porosity_)
    {
        const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);
        const volScalarField rho(this->rho());
        const volScalarField mu(this->mu());

        const HashTable<const porosityModel*> models =
            obr_.lookupClass<porosityModel>();

        if (models.empty())
        {
            WarningInFunction
                << "Porosity effects requested, but no porosity models found "
                << "in the database"
                << endl;
        }

        forAllConstIter(HashTable<const porosityModel*>, models, iter)
        {
            // non-const access required if mesh is changing
            porosityModel& pm = const_cast<porosityModel&>(*iter());

            vectorField fPTot(pm.force(U, rho, mu));

            const labelList& cellZoneIDs = pm.cellZoneIDs();

            forAll(cellZoneIDs, i)
            {
                label zoneI = cellZoneIDs[i];
                const cellZone& cZone = mesh_.cellZones()[zoneI];

                const vectorField d(mesh_.C(), cZone);
                const vectorField fP(fPTot, cZone);
                const vectorField Md(d - coordSys_.origin());

                const vectorField fDummy(Md.size(), Zero);

                applyBins(Md, fDummy, fDummy, fP, d);
            }
        }
    }

    Pstream::listCombineGather(force_, plusEqOp<vectorField>());
    Pstream::listCombineGather(moment_, plusEqOp<vectorField>());
    Pstream::listCombineScatter(force_);
    Pstream::listCombineScatter(moment_);
}


Foam::List<Foam::vector> Foam::binforces::forceEff() const
{
    return force_[0] + force_[1] + force_[2];
}


Foam::List<Foam::vector> Foam::binforces::momentEff() const
{
    return moment_[0] + moment_[1] + moment_[2];
}


bool Foam::binforces::execute()
{
    return true;
}


void Foam::binforces::log()
{
    Info<< "    sum of forces:" << nl
        << "        pressure : " << sum(force_[0]) << nl
        << "        viscous  : " << sum(force_[1]) << nl
        << "        porous   : " << sum(force_[2]) << nl
        << "    sum of moments:" << nl
        << "        pressure : " << sum(moment_[0]) << nl
        << "        viscous  : " << sum(moment_[1]) << nl
        << "        porous   : " << sum(moment_[2])
        << endl;
}

// ************************************************************************* //
