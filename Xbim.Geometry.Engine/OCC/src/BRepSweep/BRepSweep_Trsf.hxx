// Created on: 1993-06-08
// Created by: Laurent BOURESCHE
// Copyright (c) 1993-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _BRepSweep_Trsf_HeaderFile
#define _BRepSweep_Trsf_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <TopLoc_Location.hxx>
#include <Standard_Boolean.hxx>
#include <BRepSweep_NumLinearRegularSweep.hxx>
#include <TopAbs_Orientation.hxx>
class BRep_Builder;
class TopoDS_Shape;
class Sweep_NumShape;


//! This class is inherited from NumLinearRegularSweep
//! to  implement the  simple   swept primitives built
//! moving a Shape with a Trsf.  It  often is possible
//! to  build  the constructed subshapes  by  a simple
//! move of the  generating subshapes (shared topology
//! and geometry).   So two  ways of construction  are
//! proposed :
//!
//! - sharing  basis elements  (the generatrice can be
//! modified , for  example  PCurves can be added  on
//! faces);
//!
//! - copying everything.
class BRepSweep_Trsf  : public BRepSweep_NumLinearRegularSweep
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! ends  the  construction  of the   swept  primitive
  //! calling the virtual geometric functions that can't
  //! be called in the initialize.
  Standard_EXPORT void Init();
  
  //! function called to analyze the way of construction
  //! of the shapes generated by aGenS and aDirV.
  Standard_EXPORT Standard_Boolean Process (const TopoDS_Shape& aGenS, const Sweep_NumShape& aDirV);
  
  //! Builds the vertex addressed by [aGenV,aDirV], with its
  //! geometric part, but without subcomponents.
  Standard_EXPORT virtual TopoDS_Shape MakeEmptyVertex (const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirV) = 0;
  
  //! Builds the edge addressed by [aGenV,aDirE], with its
  //! geometric part, but without subcomponents.
  Standard_EXPORT virtual TopoDS_Shape MakeEmptyDirectingEdge (const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirE) = 0;
  
  //! Builds the edge addressed by [aGenE,aDirV], with its
  //! geometric part, but without subcomponents.
  Standard_EXPORT virtual TopoDS_Shape MakeEmptyGeneratingEdge (const TopoDS_Shape& aGenE, const Sweep_NumShape& aDirV) = 0;
  
  //! Sets the  parameters of the new  vertex  on the new
  //! face. The new face and  new vertex where generated
  //! from aGenF, aGenV and aDirV .
  Standard_EXPORT virtual void SetParameters (const TopoDS_Shape& aNewFace, TopoDS_Shape& aNewVertex, const TopoDS_Shape& aGenF, const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirV) = 0;
  
  //! Sets the  parameter of the new  vertex  on the new
  //! edge. The new edge and  new vertex where generated
  //! from aGenV aDirE, and aDirV.
  Standard_EXPORT virtual void SetDirectingParameter (const TopoDS_Shape& aNewEdge, TopoDS_Shape& aNewVertex, const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirE, const Sweep_NumShape& aDirV) = 0;
  
  //! Sets the  parameter of the new  vertex  on the new
  //! edge. The new edge and  new vertex where generated
  //! from aGenE, aGenV and aDirV .
  Standard_EXPORT virtual void SetGeneratingParameter (const TopoDS_Shape& aNewEdge, TopoDS_Shape& aNewVertex, const TopoDS_Shape& aGenE, const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirV) = 0;
  
  //! Builds  the face addressed  by [aGenS,aDirS], with
  //! its geometric part, but without subcomponents. The
  //! couple aGenS, aDirS can be  a "generating face and
  //! a  directing vertex" or "a   generating edge and a
  //! directing  edge".
  Standard_EXPORT virtual TopoDS_Shape MakeEmptyFace (const TopoDS_Shape& aGenS, const Sweep_NumShape& aDirS) = 0;
  
  //! Sets the PCurve for a new edge on a new face. The
  //! new edge and  the  new face were generated  using
  //! aGenF, aGenE and aDirV.
  Standard_EXPORT virtual void SetPCurve (const TopoDS_Shape& aNewFace, TopoDS_Shape& aNewEdge, const TopoDS_Shape& aGenF, const TopoDS_Shape& aGenE, const Sweep_NumShape& aDirV, const TopAbs_Orientation orien) = 0;
  
  //! Sets the PCurve for a new edge on a new face. The
  //! new edge and  the  new face were generated  using
  //! aGenE, aDirE and aDirV.
  Standard_EXPORT virtual void SetGeneratingPCurve (const TopoDS_Shape& aNewFace, TopoDS_Shape& aNewEdge, const TopoDS_Shape& aGenE, const Sweep_NumShape& aDirE, const Sweep_NumShape& aDirV, const TopAbs_Orientation orien) = 0;
  
  //! Sets the PCurve for a new edge on a new face. The
  //! new edge and  the  new face were generated  using
  //! aGenE, aDirE and aGenV.
  Standard_EXPORT virtual void SetDirectingPCurve (const TopoDS_Shape& aNewFace, TopoDS_Shape& aNewEdge, const TopoDS_Shape& aGenE, const TopoDS_Shape& aGenV, const Sweep_NumShape& aDirE, const TopAbs_Orientation orien) = 0;
  
  //! Returns   true   if  aNewSubShape    (addressed by
  //! aSubGenS  and aDirS)  must  be added  in aNewShape
  //! (addressed by aGenS and aDirS).
  Standard_EXPORT virtual Standard_Boolean GGDShapeIsToAdd (const TopoDS_Shape& aNewShape, const TopoDS_Shape& aNewSubShape, const TopoDS_Shape& aGenS, const TopoDS_Shape& aSubGenS, const Sweep_NumShape& aDirS) const = 0;
  
  //! Returns   true   if  aNewSubShape    (addressed by
  //! aGenS  and aSubDirS)  must  be added  in aNewShape
  //! (addressed by aGenS and aDirS).
  Standard_EXPORT virtual Standard_Boolean GDDShapeIsToAdd (const TopoDS_Shape& aNewShape, const TopoDS_Shape& aNewSubShape, const TopoDS_Shape& aGenS, const Sweep_NumShape& aDirS, const Sweep_NumShape& aSubDirS) const = 0;
  
  //! In  some  particular  cases  the   topology  of  a
  //! generated  face must be  composed  of  independent
  //! closed wires,  in this case  this function returns
  //! true.
  Standard_EXPORT virtual Standard_Boolean SeparatedWires (const TopoDS_Shape& aNewShape, const TopoDS_Shape& aNewSubShape, const TopoDS_Shape& aGenS, const TopoDS_Shape& aSubGenS, const Sweep_NumShape& aDirS) const = 0;
  
  //! Returns true   if aDirS   and aGenS  addresses   a
  //! resulting Shape. In some  specific cases the shape
  //! can  be    geometrically   inexsistant,  then this
  //! function returns false.
  Standard_EXPORT virtual Standard_Boolean HasShape (const TopoDS_Shape& aGenS, const Sweep_NumShape& aDirS) const = 0;
  
  //! Returns  true if  the geometry   of  aGenS is  not
  //! modified by the trsf of the BRepSweep Trsf.
  Standard_EXPORT virtual Standard_Boolean IsInvariant (const TopoDS_Shape& aGenS) const = 0;
  
  //! Called to propagate the continuity of  every vertex
  //! between two edges of the  generating wire  aGenS on
  //! the generated edge and faces.
  Standard_EXPORT void SetContinuity (const TopoDS_Shape& aGenS, const Sweep_NumShape& aDirS);




protected:

  
  //! Initialize  the Trsf BrepSweep, if  aCopy  is true
  //! the  basis elements  are    shared  as   often  as
  //! possible, else everything is copied.
  Standard_EXPORT BRepSweep_Trsf(const BRep_Builder& aBuilder, const TopoDS_Shape& aGenShape, const Sweep_NumShape& aDirWire, const TopLoc_Location& aLocation, const Standard_Boolean aCopy);


  TopLoc_Location myLocation;
  Standard_Boolean myCopy;


private:





};







#endif // _BRepSweep_Trsf_HeaderFile
