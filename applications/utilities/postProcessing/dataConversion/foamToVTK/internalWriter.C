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
    This file is a derivative work of OpenFOAM.

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
      - Derived from the patches for blueCFD 2.1 and 2.2. which in turn were derived
        from 2.0 and 1.7.
      - Always open the files in binary mode, because of how things work on 
        Windows.
        - Note: This modification is hard to stipulate who implemented this
                first. Symscape's port only began integrating this fix after
                blueCFD 1.7-2 was released with this sort of specific fix.
                But it was Symscape that first implemented this kind of
                "binary" fix in OpenFOAM's core code.

\*---------------------------------------------------------------------------*/

#include "internalWriter.H"
#include "writeFuns.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::internalWriter::internalWriter
(
    const vtkMesh& vMesh,
    const bool binary,
    const fileName& fName
)
:
    vMesh_(vMesh),
    binary_(binary),
    fName_(fName),
    os_(fName.c_str(),
        std::ios_base::out|std::ios_base::binary) //a must for Windows!
{
    const fvMesh& mesh = vMesh_.mesh();
    const vtkTopo& topo = vMesh_.topo();

    // Write header
    writeFuns::writeHeader(os_, binary_, mesh.time().caseName());
    os_ << "DATASET UNSTRUCTURED_GRID" << std::endl;


    //------------------------------------------------------------------
    //
    // Write topology
    //
    //------------------------------------------------------------------

    const labelList& addPointCellLabels = topo.addPointCellLabels();
    const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();

    os_ << "POINTS " << nTotPoints << " float" << std::endl;

    DynamicList<floatScalar> ptField(3*nTotPoints);

    writeFuns::insert(mesh.points(), ptField);

    const pointField& ctrs = mesh.cellCentres();
    forAll(addPointCellLabels, api)
    {
        writeFuns::insert(ctrs[addPointCellLabels[api]], ptField);
    }
    writeFuns::write(os_, binary_, ptField);


    //
    // Write cells
    //

    const labelListList& vtkVertLabels = topo.vertLabels();

    // Count total number of vertices referenced.
    label nFaceVerts = 0;

    forAll(vtkVertLabels, cellI)
    {
        nFaceVerts += vtkVertLabels[cellI].size() + 1;
    }

    os_ << "CELLS " << vtkVertLabels.size() << ' ' << nFaceVerts << std::endl;

    DynamicList<label> vertLabels(nFaceVerts);

    forAll(vtkVertLabels, cellI)
    {
        const labelList& vtkVerts = vtkVertLabels[cellI];

        vertLabels.append(vtkVerts.size());

        writeFuns::insert(vtkVerts, vertLabels);
    }
    writeFuns::write(os_, binary_, vertLabels);


    const labelList& vtkCellTypes = topo.cellTypes();

    os_ << "CELL_TYPES " << vtkCellTypes.size() << std::endl;

    // Make copy since writing might swap stuff.
    DynamicList<label> cellTypes(vtkCellTypes.size());

    writeFuns::insert(vtkCellTypes, cellTypes);

    writeFuns::write(os_, binary_, cellTypes);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::internalWriter::writeCellIDs()
{
    const fvMesh& mesh = vMesh_.mesh();
    const vtkTopo& topo = vMesh_.topo();
    const labelList& vtkCellTypes = topo.cellTypes();
    const labelList& superCells = topo.superCells();

    // Cell ids first
    os_ << "cellID 1 " << vtkCellTypes.size() << " int" << std::endl;

    labelList cellId(vtkCellTypes.size());
    label labelI = 0;


    if (vMesh_.useSubMesh())
    {
        const labelList& cMap = vMesh_.subsetter().cellMap();

        forAll(mesh.cells(), cellI)
        {
            cellId[labelI++] = cMap[cellI];
        }
        forAll(superCells, superCellI)
        {
            label origCellI = cMap[superCells[superCellI]];

            cellId[labelI++] = origCellI;
        }
    }
    else
    {
        forAll(mesh.cells(), cellI)
        {
            cellId[labelI++] = cellI;
        }
        forAll(superCells, superCellI)
        {
            label origCellI = superCells[superCellI];

            cellId[labelI++] = origCellI;
        }
    }

    writeFuns::write(os_, binary_, cellId);
}


// ************************************************************************* //
