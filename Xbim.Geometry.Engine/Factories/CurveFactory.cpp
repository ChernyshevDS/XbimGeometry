#include "CurveFactory.h"
#include "../Exceptions/XbimGeometryFactoryException.h"

#include <GeomLib_Tool.hxx>

#include "../BRep/XbimLine.h"
#include "../BRep/XbimCircle.h"
#include "../BRep/XbimTrimmedCurve.h"
#include "../BRep/XbimTrimmedCurve2d.h"
/*
The approach of the curve factory is to build all curves as IXCurve using the build method.
This will ensure correct dimensionality of the curves is maintained
Most curve types can have a 2D or a 3D variant, Ifc hides this in the Dim method.
Any 3D shape can be built from a 2D definition with the Z coordinate set to Zero
It is nor permitted to create 2D shapes from 3D definitions and an exception is thrown.
Managed code is used to navigate the definitions and provide the framework for the unmanaged code to build the native curves
All operations where an OCC excpetion will be thrown are implemented in unmanaged code. (NCurveFactory)
These exceptions are caunght and logged in the managed code
Unmanaged build methods reurn a null handle to the specified geometry type when a critical or error exception has been thrown
*/
using namespace Xbim::Geometry::Exceptions;
using namespace Xbim::Ifc4::MeasureResource;
using namespace Xbim::Geometry::BRep;
using namespace Xbim::Common::Metadata;
namespace Xbim
{
	namespace Geometry
	{
		namespace Factories
		{

			IXCurve^ CurveFactory::Build(IIfcCurve^ curve)
			{
				int dim = (int)curve->Dim;

				XCurveType curveType;

				if (dim == 2)
				{
					Handle(Geom2d_Curve) hCurve2d = BuildGeom2d(curve, curveType);
					if (hCurve2d.IsNull()) throw gcnew XbimGeometryFactoryException("Failed to build curve");
					return BuildXCurve(hCurve2d, curveType);
				}
				else
				{
					Handle(Geom_Curve) hCurve = BuildGeom3d(curve, curveType);
					if (hCurve.IsNull()) throw gcnew XbimGeometryFactoryException("Failed to build curve");
					return BuildXCurve(hCurve, curveType);
				}
			}

			Handle(Geom2d_Curve) CurveFactory::BuildGeom2d(IIfcCurve^ curve, XCurveType% curveType)
			{
				if (!Enum::TryParse<XCurveType>(curve->ExpressType->ExpressName, curveType))
					throw gcnew XbimGeometryFactoryException("Unsupported curve type: " + curve->ExpressType->ExpressName);
				switch (curveType)
				{
					/*case Xbim::Geometry::Abstractions::XCurveType::BoundaryCurve:
						return  Build2d(dynamic_cast<IIfcBoundedCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::BSplineCurveWithKnots:
						return Build2d(dynamic_cast<IIfcBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Circle:
						return Build2d(dynamic_cast<IIfcCircle^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurve:
						return Build2d(dynamic_cast<IIfcCompositeCurve^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurveOnSurface:
						return Build2d(dynamic_cast<IIfcCompositeCurveOnSurface^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::Ellipse:
						return Build2d(dynamic_cast<IIfcEllipse^>(curve)) );
					case Xbim::Geometry::Abstractions::XCurveType::IndexedPolyCurve:
						return Build2d(dynamic_cast<IIfcIndexedPolyCurve^>(curve)));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcLine:
					return BuildGeom2d(dynamic_cast<IIfcLine^>(curve));
					/*case Xbim::Geometry::Abstractions::XCurveType::OffsetCurve2D:
						return Build2d(dynamic_cast<IIfcOffsetCurve2D^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Pcurve:
						return Build2d(dynamic_cast<IIfcPcurve^>(curve)) ;
					case Xbim::Geometry::Abstractions::XCurveType::Polyline:
						return Build2d(dynamic_cast<IIfcPolyline^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::RationalBSplineCurveWithKnots:
						return Build2d(dynamic_cast<IIfcRationalBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::SurfaceCurve:
						return Build2d(dynamic_cast<IIfcSurfaceCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcTrimmedCurve:
					return BuildGeom2d(dynamic_cast<IIfcTrimmedCurve^>(curve));
					break;
				default:
					throw gcnew XbimGeometryFactoryException("Unsupported 2d curve type");
				}
				throw gcnew XbimGeometryFactoryException("Unsupported 2d curve type");
			}


			Handle(Geom_Curve) CurveFactory::BuildGeom3d(IIfcCurve^ curve, XCurveType% curveType)
			{


				if (!Enum::TryParse<XCurveType>(curve->ExpressType->ExpressName, curveType))
					throw gcnew XbimGeometryFactoryException("Unsupported curve type: " + curve->ExpressType->ExpressName);

				switch (curveType)
				{
					/*case Xbim::Geometry::Abstractions::XCurveType::BoundaryCurve:
						return Build3d(dynamic_cast<IIfcBoundedCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::BSplineCurveWithKnots:
						return Build3d(dynamic_cast<IIfcBSplineCurveWithKnots^>(curve));*/
					case Xbim::Geometry::Abstractions::XCurveType::IfcCircle:
						return BuildGeom3d(dynamic_cast<IIfcCircle^>(curve));
					/*case Xbim::Geometry::Abstractions::XCurveType::CompositeCurve:
						return Build3d(dynamic_cast<IIfcCompositeCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurveOnSurface:
						return Build3d(dynamic_cast<IIfcCompositeCurveOnSurface^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Ellipse:
						return Build3d(dynamic_cast<IIfcEllipse^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::IndexedPolyCurve:
						return Build3d(dynamic_cast<IIfcIndexedPolyCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcLine:
					return BuildGeom3d(dynamic_cast<IIfcLine^>(curve));
					/*
					case Xbim::Geometry::Abstractions::XCurveType::OffsetCurve3D:
						return Build2d(dynamic_cast<IIfcOffsetCurve3D^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Pcurve:
						return Build3d(dynamic_cast<IIfcPcurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Polyline:
						return Build3d(dynamic_cast<IIfcPolyline^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::RationalBSplineCurveWithKnots:
						return Build3d(dynamic_cast<IIfcRationalBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::SurfaceCurve:
						return Build3d(dynamic_cast<IIfcSurfaceCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcTrimmedCurve:
					return BuildGeom3d(dynamic_cast<IIfcTrimmedCurve^>(curve));
					break;
				default:
					throw gcnew XbimGeometryFactoryException("Unsupported curve type");
				}
				throw gcnew XbimGeometryFactoryException("Unsupported curve type");
			}

			IXCurve^ CurveFactory::BuildXCurve(Handle(Geom_Curve) curve, XCurveType curveType)
			{
				switch (curveType)
				{
					/*case Xbim::Geometry::Abstractions::XCurveType::BoundaryCurve:
						return Build3d(dynamic_cast<IIfcBoundedCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::BSplineCurveWithKnots:
						return Build3d(dynamic_cast<IIfcBSplineCurveWithKnots^>(curve));*/
					case Xbim::Geometry::Abstractions::XCurveType::IfcCircle:
						return gcnew XbimCircle(Handle(Geom_Circle)::DownCast(curve));
					/*case Xbim::Geometry::Abstractions::XCurveType::CompositeCurve:
						return Build3d(dynamic_cast<IIfcCompositeCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurveOnSurface:
						return Build3d(dynamic_cast<IIfcCompositeCurveOnSurface^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Ellipse:
						return Build3d(dynamic_cast<IIfcEllipse^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::IndexedPolyCurve:
						return Build3d(dynamic_cast<IIfcIndexedPolyCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcLine:
					return gcnew Xbim::Geometry::BRep::XbimLine(Handle(Geom_LineWithMagnitude)::DownCast(curve));
					/*
					case Xbim::Geometry::Abstractions::XCurveType::OffsetCurve3D:
						return Build2d(dynamic_cast<IIfcOffsetCurve3D^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Pcurve:
						return Build3d(dynamic_cast<IIfcPcurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Polyline:
						return Build3d(dynamic_cast<IIfcPolyline^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::RationalBSplineCurveWithKnots:
						return Build3d(dynamic_cast<IIfcRationalBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::SurfaceCurve:
						return Build3d(dynamic_cast<IIfcSurfaceCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcTrimmedCurve:
					return gcnew XbimTrimmedCurve(Handle(Geom_TrimmedCurve)::DownCast(curve));
					break;
				default:
					throw gcnew XbimGeometryFactoryException("Unsupported curve type");
				}
				throw gcnew XbimGeometryFactoryException("Unsupported curve type");
			}

			IXCurve^ CurveFactory::BuildXCurve(Handle(Geom2d_Curve) curve, XCurveType curveType)
			{
				switch (curveType)
				{
					/*case Xbim::Geometry::Abstractions::XCurveType::BoundaryCurve:
						return  Build2d(dynamic_cast<IIfcBoundedCurve^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::BSplineCurveWithKnots:
						return Build2d(dynamic_cast<IIfcBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Circle:
						return Build2d(dynamic_cast<IIfcCircle^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurve:
						return Build2d(dynamic_cast<IIfcCompositeCurve^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::CompositeCurveOnSurface:
						return Build2d(dynamic_cast<IIfcCompositeCurveOnSurface^>(curve)));
					case Xbim::Geometry::Abstractions::XCurveType::Ellipse:
						return Build2d(dynamic_cast<IIfcEllipse^>(curve)) );
					case Xbim::Geometry::Abstractions::XCurveType::IndexedPolyCurve:
						return Build2d(dynamic_cast<IIfcIndexedPolyCurve^>(curve)));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcLine:
					return gcnew XbimLine2d(Handle(Geom2d_LineWithMagnitude)::DownCast(curve));
					/*case Xbim::Geometry::Abstractions::XCurveType::OffsetCurve2D:
						return Build2d(dynamic_cast<IIfcOffsetCurve2D^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::Pcurve:
						return Build2d(dynamic_cast<IIfcPcurve^>(curve)) ;
					case Xbim::Geometry::Abstractions::XCurveType::Polyline:
						return Build2d(dynamic_cast<IIfcPolyline^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::RationalBSplineCurveWithKnots:
						return Build2d(dynamic_cast<IIfcRationalBSplineCurveWithKnots^>(curve));
					case Xbim::Geometry::Abstractions::XCurveType::SurfaceCurve:
						return Build2d(dynamic_cast<IIfcSurfaceCurve^>(curve));*/
				case Xbim::Geometry::Abstractions::XCurveType::IfcTrimmedCurve:
					return gcnew XbimTrimmedCurve2d(Handle(Geom2d_TrimmedCurve)::DownCast(curve));
					break;
				default:
					throw gcnew XbimGeometryFactoryException("Unsupported 2d curve type");
				}
				throw gcnew XbimGeometryFactoryException("Unsupported 2d curve type");
			}



			/*IXLine^ CurveFactory::BuildLine3d(IIfcLine^ ifcLine)
			{
				Handle(Geom_Line) hl = BuildLine3d(ifcLine);
				if (!hl.IsNull())
					return gcnew Xbim::Geometry::BRep::XbimLine(hl, ifcLine->Dir->Magnitude);
				else
					throw gcnew XbimGeometryFactoryException("Failed to build Line");
			}*/
			Handle(Geom_LineWithMagnitude) CurveFactory::BuildGeom3d(IIfcLine^ ifcLine)
			{
				gp_Pnt origin = GpFactory->BuildPoint(ifcLine->Pnt);
				gp_Dir direction = GpFactory->BuildDirection(ifcLine->Dir->Orientation);
				return Ptr()->BuildLine3d(origin, direction, ifcLine->Dir->Magnitude);

			}

			Handle(Geom2d_LineWithMagnitude) CurveFactory::BuildGeom2d(IIfcLine^ ifcLine)
			{
				if (2 != (int)ifcLine->Dim) throw gcnew XbimGeometryFactoryException("Cannot build a 2D curve from a 3D curve");
				gp_Pnt2d origin = GpFactory->BuildPoint2d(ifcLine->Pnt);
				gp_Dir2d direction = GpFactory->BuildDirection2d(ifcLine->Dir->Orientation);
				return Ptr()->BuildLine2d(origin, direction, ifcLine->Dir->Magnitude);
			}

			Handle(Geom_Circle) CurveFactory::BuildGeom3d(IIfcCircle^ ifcCircle)
			{
				IIfcAxis2Placement3D^ axis3d = dynamic_cast<IIfcAxis2Placement3D^>(ifcCircle->Position);
				if (axis3d == nullptr) throw gcnew XbimGeometryFactoryException("Cannot build a 3D curve with 2D placement");
				gp_Ax2 pos = GpFactory->BuildAxis2Placement(axis3d);
				return Ptr()->BuildCircle3d(pos, ifcCircle->Radius);

			}

			Handle(Geom2d_Circle) CurveFactory::BuildGeom2d(IIfcCircle^ ifcCircle)
			{
				if (2 != (int)ifcCircle->Dim) throw gcnew XbimGeometryFactoryException("Cannot build a 2D curve from a 3D curve");
				IIfcAxis2Placement2D^ axis2d = dynamic_cast<IIfcAxis2Placement2D^>(ifcCircle->Position);
				if (axis2d == nullptr) throw gcnew XbimGeometryFactoryException("Cannot build a 2D curve with 3D placement");
				gp_Ax2d pos = GpFactory->BuildAxis2Placement2d(axis2d);
				return Ptr()->BuildCircle2d(pos, ifcCircle->Radius);
			}


			Handle(Geom_TrimmedCurve) CurveFactory::BuildGeom3d(IIfcTrimmedCurve^ ifcTrimmedCurve)
			{
				//Validation
				if (dynamic_cast<IIfcBoundedCurve^>(ifcTrimmedCurve->BasisCurve))
					throw gcnew XbimGeometryFactoryException("Ifc Formal Proposition: NoTrimOfBoundedCurves. Already bounded curves shall not be trimmed.");
				XCurveType curveType;
				Handle(Geom_Curve) basisCurve = BuildGeom3d(ifcTrimmedCurve->BasisCurve, curveType);
				if (!basisCurve.IsNull())
				{
					bool isConic = (dynamic_cast<IIfcConic^>(ifcTrimmedCurve->BasisCurve) != nullptr);
					bool isLine = (dynamic_cast<IIfcLine^>(ifcTrimmedCurve->BasisCurve) != nullptr);
					bool isEllipse = (dynamic_cast<IIfcEllipse^>(ifcTrimmedCurve->BasisCurve) != nullptr);

					//get the parametric values
					IfcTrimmingPreference trimPref = ifcTrimmedCurve->MasterRepresentation;

					bool trim_cartesian = (ifcTrimmedCurve->MasterRepresentation == IfcTrimmingPreference::CARTESIAN);

					double u1 = double::NegativeInfinity, u2 = double::PositiveInfinity;
					IIfcCartesianPoint^ cp1 = nullptr;
					IIfcCartesianPoint^ cp2 = nullptr;
					gp_Pnt p1, p2;
					for each (IIfcTrimmingSelect ^ trim in ifcTrimmedCurve->Trim1)
					{
						if (dynamic_cast<IIfcCartesianPoint^>(trim))cp1 = (IIfcCartesianPoint^)trim;
						else u1 = (double)(IfcParameterValue)trim; //its parametric	
					}
					for each (IIfcTrimmingSelect ^ trim in ifcTrimmedCurve->Trim2)
					{
						if (dynamic_cast<IIfcCartesianPoint^>(trim))cp2 = (IIfcCartesianPoint^)trim;
						else u2 = (double)(IfcParameterValue)trim; //its parametric	
					}

					if ((trim_cartesian && cp1 != nullptr && cp2 != nullptr) ||
						(cp1 != nullptr && cp2 != nullptr &&
							(double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)))) //we want cartesian and we have both or we don't have both parameters but have cartesians
					{
						if (!GeomLib_Tool::Parameter(basisCurve, p1, _pointOnCurveTolerance, u1))
							throw gcnew XbimGeometryFactoryException("Trim Point1 is not on the basis curve");
						if (!GeomLib_Tool::Parameter(basisCurve, p2, _pointOnCurveTolerance, u2))
							throw gcnew XbimGeometryFactoryException("Trim Point2 is not on the basis curve");
					}
					else if (double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)) //non-compliant
						throw gcnew XbimGeometryFactoryException("Ifc Formal Proposition: TrimValuesConsistent. Either a single value is specified for Trim, or the two trimming values are of different type (point and parameter)");
					else //we prefer to use parameters but need to adjust
					{
						if (isConic)
						{
							u1 *= _radiansFactor; //correct to radians
							u2 *= _radiansFactor; //correct to radians
						}
					}
					if (double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)) //sanity check in case the logic has missed a situtation
						throw gcnew XbimGeometryFactoryException("Error converting Ifc Trim Points");
					if (u1 == u2) //if the parameters are the same trimming will fail
						throw gcnew XbimGeometryFactoryException("Parametric Trim Points are equal and will result in an empty curve");

					return Ptr()->BuildTrimmedCurve3d(basisCurve, u1, u2, ifcTrimmedCurve->SenseAgreement);
				}
				else
					throw gcnew XbimGeometryFactoryException("Failed to build Trimmed Basis Curve");

			}

			Handle(Geom2d_TrimmedCurve) CurveFactory::BuildGeom2d(IIfcTrimmedCurve^ ifcTrimmedCurve)
			{
				//Validation
				if (dynamic_cast<IIfcBoundedCurve^>(ifcTrimmedCurve->BasisCurve))
					throw gcnew XbimGeometryFactoryException("Ifc Formal Proposition: NoTrimOfBoundedCurves. Already bounded curves shall not be trimmed.");
				XCurveType curveType;
				Handle(Geom2d_Curve) basisCurve = BuildGeom2d(ifcTrimmedCurve->BasisCurve, curveType);
				if (!basisCurve.IsNull())
				{
					bool isConic = (dynamic_cast<IIfcConic^>(ifcTrimmedCurve->BasisCurve) != nullptr);
					bool isLine = (dynamic_cast<IIfcLine^>(ifcTrimmedCurve->BasisCurve) != nullptr);
					bool isEllipse = (dynamic_cast<IIfcEllipse^>(ifcTrimmedCurve->BasisCurve) != nullptr);

					//get the parametric values
					IfcTrimmingPreference trimPref = ifcTrimmedCurve->MasterRepresentation;

					bool trim_cartesian = (ifcTrimmedCurve->MasterRepresentation == IfcTrimmingPreference::CARTESIAN);

					double u1 = double::NegativeInfinity, u2 = double::PositiveInfinity;
					IIfcCartesianPoint^ cp1 = nullptr;
					IIfcCartesianPoint^ cp2 = nullptr;
					gp_Pnt2d p1, p2;
					for each (IIfcTrimmingSelect ^ trim in ifcTrimmedCurve->Trim1)
					{
						if (dynamic_cast<IIfcCartesianPoint^>(trim))cp1 = (IIfcCartesianPoint^)trim;
						else u1 = (double)(IfcParameterValue)trim; //its parametric	
					}
					for each (IIfcTrimmingSelect ^ trim in ifcTrimmedCurve->Trim2)
					{
						if (dynamic_cast<IIfcCartesianPoint^>(trim))cp2 = (IIfcCartesianPoint^)trim;
						else u2 = (double)(IfcParameterValue)trim; //its parametric	
					}

					if ((trim_cartesian && cp1 != nullptr && cp2 != nullptr) ||
						(cp1 != nullptr && cp2 != nullptr &&
							(double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)))) //we want cartesian and we have both or we don't have both parameters but have cartesians
					{
						if (!GeomLib_Tool::Parameter(basisCurve, p1, _pointOnCurveTolerance, u1))
							throw gcnew XbimGeometryFactoryException("Trim Point1 is not on the basis curve");
						if (!GeomLib_Tool::Parameter(basisCurve, p2, _pointOnCurveTolerance, u2))
							throw gcnew XbimGeometryFactoryException("Trim Point2 is not on the basis curve");
					}
					else if (double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)) //non-compliant
						throw gcnew XbimGeometryFactoryException("Ifc Formal Proposition: TrimValuesConsistent. Either a single value is specified for Trim, or the two trimming values are of different type (point and parameter)");
					else //we prefer to use parameters but need to adjust
					{
						if (isConic)
						{
							u1 *= _radiansFactor; //correct to radians
							u2 *= _radiansFactor; //correct to radians
						}
						else if (isLine) //need to consider the magnitude of a parametric unit
						{
							double magnitude = ((IIfcLine^)ifcTrimmedCurve->BasisCurve)->Dir->Magnitude;
							u1 *= magnitude;
							u2 *= magnitude;
						}
					}
					if (double::IsNegativeInfinity(u1) || double::IsPositiveInfinity(u2)) //sanity check in case the logic has missed a situtation
						throw gcnew XbimGeometryFactoryException("Error converting Ifc Trim Points");
					if (u1 == u2) //if the parameters are the same trimming will fail
						throw gcnew XbimGeometryFactoryException("Parametric Trim Points are equal and will result in an empty curve");

					return Ptr()->BuildTrimmedCurve2d(basisCurve, u1, u2, ifcTrimmedCurve->SenseAgreement);
				}
				else
					throw gcnew XbimGeometryFactoryException("Failed to build Trimmed Basis Curve");

			}
		}
	}
}
