/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
------------------------------------------------------------------------------
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

Modifications
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Always open the files in binary mode, because of how things work on 
        Windows.

\*---------------------------------------------------------------------------*/

#include "writePointSet.H"
#include "OFstream.H"
#include "writeFuns.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void writePointSet
(
    const bool binary,
    const primitiveMesh& mesh,
    const topoSet& set,
    const fileName& fileName
)
{
    // Use binary mode in case we write binary.
    // Causes windows reading to fail if we don't
    std::ofstream pStream(fileName.c_str(), 
                       ios_base::out|ios_base::binary);

    pStream
        << "# vtk DataFile Version 2.0" << std::endl
        << set.name() << std::endl;
    if (binary)
    {
        pStream << "BINARY" << std::endl;
    }
    else
    {
        pStream << "ASCII" << std::endl;
    }
    pStream << "DATASET POLYDATA" << std::endl;


    //------------------------------------------------------------------
    //
    // Write topology
    //
    //------------------------------------------------------------------


    labelList pointLabels(set.toc());

    pointField setPoints(mesh.points(), pointLabels);

    // Write points

    pStream << "POINTS " << pointLabels.size() << " float" << std::endl;

    DynamicList<floatScalar> ptField(3*pointLabels.size());

    writeFuns::insert(setPoints, ptField);

    writeFuns::write(pStream, binary, ptField);


    //-----------------------------------------------------------------
    //
    // Write data
    //
    //-----------------------------------------------------------------

    // Write pointID

    pStream
        << "POINT_DATA " << pointLabels.size() << std::endl
        << "FIELD attributes 1" << std::endl;

    // Cell ids first
    pStream << "pointID 1 " << pointLabels.size() << " int" << std::endl;

    writeFuns::write(pStream, binary, pointLabels);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
