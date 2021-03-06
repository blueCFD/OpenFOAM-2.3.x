/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "surfaceFilmModel.H"
#include "fvMesh.H"
#include "Time.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<surfaceFilmModel> surfaceFilmModel::New
(
    const fvMesh& mesh,
    const dimensionedVector& g,
    const word& regionType
)
{
    word modelType;

    {
        IOdictionary surfaceFilmPropertiesDict
        (
            IOobject
            (
                regionType + "Properties",
                mesh.time().constant(),
                mesh,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        surfaceFilmPropertiesDict.lookup("surfaceFilmModel") >> modelType;
    }

    Info<< "Selecting surfaceFilmModel " << modelType << endl;

    meshConstructorTable::iterator cstrIter =
        meshConstructorTablePtr_->find(modelType);

    if (cstrIter == meshConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "surfaceFilmModel::New(const fvMesh&, const dimensionedVector&)"
        )   << "Unknown surfaceFilmModel type " << modelType
            << nl << nl << "Valid surfaceFilmModel types are:" << nl
            << meshConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<surfaceFilmModel>
    (
        cstrIter()
        (
            modelType,
            mesh,
            g,
            regionType
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace Foam

// ************************************************************************* //
