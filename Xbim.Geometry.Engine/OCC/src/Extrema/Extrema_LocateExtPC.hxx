// Created on: 1991-02-26
// Created by: Isabelle GRIGNON
// Copyright (c) 1991-1999 Matra Datavision
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

#ifndef _Extrema_LocateExtPC_HeaderFile
#define _Extrema_LocateExtPC_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <Extrema_POnCurv.hxx>
#include <Standard_Address.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <Extrema_LocEPCOfLocateExtPC.hxx>
#include <Extrema_ELPCOfLocateExtPC.hxx>
#include <GeomAbs_CurveType.hxx>
#include <Standard_Integer.hxx>
#include <Extrema_SequenceOfPOnCurv.hxx>
class Standard_DomainError;
class StdFail_NotDone;
class Adaptor3d_Curve;
class Extrema_CurveTool;
class Extrema_ExtPElC;
class gp_Pnt;
class gp_Vec;
class Extrema_POnCurv;
class Extrema_ELPCOfLocateExtPC;
class Extrema_EPCOfELPCOfLocateExtPC;
class Extrema_LocEPCOfLocateExtPC;
class Extrema_PCLocFOfLocEPCOfLocateExtPC;



class Extrema_LocateExtPC 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT Extrema_LocateExtPC();
  
  //! Calculates the distance with a close point.
  //! The close point is defined by the parameter value
  //! U0.
  //! The function F(u)=distance(P,C(u)) has an extremum
  //! when g(u)=dF/du=0. The algorithm searches a zero
  //! near the close point.
  //! TolF is used to decide to stop the iterations.
  //! At the nth iteration, the criteria is:
  //! abs(Un - Un-1) < TolF.
  Standard_EXPORT Extrema_LocateExtPC(const gp_Pnt& P, const Adaptor3d_Curve& C, const Standard_Real U0, const Standard_Real TolF);
  
  //! Calculates the distance with a close point.
  //! The close point is defined by the parameter value
  //! U0.
  //! The function F(u)=distance(P,C(u)) has an extremum
  //! when g(u)=dF/du=0. The algorithm searches a zero
  //! near the close point.
  //! Zeros are searched between Umin et Usup.
  //! TolF is used to decide to stop the iterations.
  //! At the nth iteration, the criteria is:
  //! abs(Un - Un-1) < TolF.
  Standard_EXPORT Extrema_LocateExtPC(const gp_Pnt& P, const Adaptor3d_Curve& C, const Standard_Real U0, const Standard_Real Umin, const Standard_Real Usup, const Standard_Real TolF);
  
  //! sets the fields of the algorithm.
  Standard_EXPORT void Initialize (const Adaptor3d_Curve& C, const Standard_Real Umin, const Standard_Real Usup, const Standard_Real TolF);
  
  Standard_EXPORT void Perform (const gp_Pnt& P, const Standard_Real U0);
  
  //! Returns True if the distance is found.
  Standard_EXPORT Standard_Boolean IsDone() const;
  
  //! Returns the value of the extremum square distance.
  Standard_EXPORT Standard_Real SquareDistance() const;
  
  //! Returns True if the extremum distance is a minimum.
  Standard_EXPORT Standard_Boolean IsMin() const;
  
  //! Returns the point of the extremum distance.
  Standard_EXPORT const Extrema_POnCurv& Point() const;




protected:





private:



  Extrema_POnCurv mypp;
  Standard_Address myC;
  Standard_Real mydist2;
  Standard_Boolean myismin;
  Standard_Boolean myDone;
  Standard_Real myumin;
  Standard_Real myusup;
  Standard_Real mytol;
  Extrema_LocEPCOfLocateExtPC myLocExtPC;
  Extrema_ELPCOfLocateExtPC myExtremPC;
  GeomAbs_CurveType type;
  Standard_Integer numberext;


};







#endif // _Extrema_LocateExtPC_HeaderFile
