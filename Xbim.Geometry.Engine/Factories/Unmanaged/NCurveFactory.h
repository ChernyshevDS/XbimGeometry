#pragma once
#include <gp_Pnt.hxx>

#include <Geom_Circle.hxx>
#include <Geom2d_Circle.hxx>
#include "../../BRep/OccExtensions/Geom2d_LineWithMagnitude.h"
#include "../../BRep/OccExtensions/Geom_LineWithMagnitude.h"
#include <Geom_TrimmedCurve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include "../../Services/LoggingService.h"
using namespace Xbim::Geometry::Services;
namespace Xbim
{
	namespace Geometry
	{
		namespace Factories
		{
			namespace Unmanaged
			{
				class NCurveFactory
				{
				public:
					
					LoggingServiceNative* pLoggingService;
					NCurveFactory(LoggingServiceNative* loggingService)
					{
						pLoggingService = loggingService;
					};
					Handle(Geom2d_LineWithMagnitude) BuildLine2d(gp_Pnt2d pnt, gp_Dir2d dir, double magnitude);
					Handle(Geom_LineWithMagnitude) BuildLine3d(gp_Pnt pnt, gp_Dir dir, double magnitude);

					Handle(Geom_Circle) BuildCircle3d(gp_Ax2 axis, double radius);
					Handle(Geom2d_Circle) BuildCircle2d(gp_Ax2d axis, double radius);

					Handle(Geom2d_TrimmedCurve) BuildTrimmedCurve2d(Handle(Geom2d_Curve) basisCurve, double u1, double u2, bool sense);
					Handle(Geom_TrimmedCurve) BuildTrimmedCurve3d(Handle(Geom_Curve) basisCurve, double u1, double u2, bool sense);
				};
			}
		}
	}
}


