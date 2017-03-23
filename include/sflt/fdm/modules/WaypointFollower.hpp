

#ifndef __sflt_fdm_WaypointFollower_H__
#define __sflt_fdm_WaypointFollower_H__

#include "sflt/fdm/modules/FDMModule.hpp"

#include <vector>

namespace sflt {
namespace xml {
class Node;
}
namespace fdm {

//------------------------------------------------------------------------------
// Class: Waypoint
//------------------------------------------------------------------------------
class Waypoint
{
 public:
   double radLat{};
   double radLon{};
   double meterAlt{};
   double mpsSpeed{};
   double radHeading{};
};

//------------------------------------------------------------------------------
// Class: WaypointFollower
//------------------------------------------------------------------------------
class WaypointFollower : public FDMModule
{
 public:
   WaypointFollower(FDMGlobals* globals, double frameRate);
   virtual ~WaypointFollower() = default;

   // module interface
   void initialize(xml::Node* node);
   void update(const double timestep);

   void loadWaypoint();
   void setState(bool isOn);
   void setCurrentWp(int num);
   int getCurrentWp();
   int getNumWaypoints();
   void clearAllWaypoints();
   void addWaypoint(const double radLat, const double radLon,
                    const double meterAlt, const double mpsSpeed,
                    const double radBearing);

 protected:
   std::vector<Waypoint> waypoints;
   Waypoint* currentWp{};
   int wpNum{};

   double altTol{};
   double distTol{};
   double azTol{};

   bool isOn{};

   enum class PathType { DIRECT, BEARING };

   PathType cmdPathType;
};
}
}

#endif