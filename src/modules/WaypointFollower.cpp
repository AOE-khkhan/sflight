
#include "modules/WaypointFollower.hpp"

#include "FDMGlobals.hpp"
#include "Earth.hpp"
#include "UnitConvert.hpp"
#include "constants.hpp"
#include "xml/node_utils.hpp"
#include "UnitConvert.hpp"

#include <cmath>

namespace sf
{

WaypointFollower::WaypointFollower(FDMGlobals *globals, double frameRate)
    : FDMModule(globals, frameRate)
{
}

WaypointFollower::~WaypointFollower() {}

void WaypointFollower::initialize(Node* node)
{
   Node *tmp = node->getChild("WaypointFollower");

   isOn = getBool(tmp, "WaypointFollow", true);

   cmdPathType = (get(tmp, "PathType", "DIRECT") == "BEARING") ? PathType::BEARING : PathType::DIRECT;

   std::vector<Node *> wps = getList(tmp->getChild("WaypointList"), "Waypoint");

   for (unsigned int i = 0; i < wps.size(); i++)
   {
      Node* wp = wps[i];
      addWaypoint(UnitConvert::toRads(getDouble(wp, "Lat", 0)),
                  UnitConvert::toRads(getDouble(wp, "Lon", 0)),
                  UnitConvert::toMeters(getDouble(wp, "Alt", 0)),
                  UnitConvert::toMPS(getDouble(wp, "Speed", 0)),
                  UnitConvert::toRads(getDouble(wp, "Heading", 0)));
   }

   if (isOn)
   {
      wpNum = 0;
      currentWp = nullptr;

      altTol = 10.;
      distTol = UnitConvert::NMtoEarthRadians(0.1);
      azTol = UnitConvert::toRads(1);

      globals->autoPilotCmds.setAltHoldOn(true);
      globals->autoPilotCmds.setHdgHoldOn(true);
      globals->autoPilotCmds.setAutoPilotOn(true);
      globals->autoPilotCmds.setAutoThrottleOn(true);

      loadWaypoint();
   }
}

void WaypointFollower::setState(bool isOn)
{
   if (isOn)
   {
      globals->autoPilotCmds.setAltHoldOn(isOn);
      globals->autoPilotCmds.setHdgHoldOn(isOn);
      globals->autoPilotCmds.setAutoPilotOn(isOn);
      globals->autoPilotCmds.setAutoThrottleOn(isOn);
   }
   this->isOn = isOn;
}

void WaypointFollower::update(double timestep)
{
   if (!isOn)
      return;

   if (currentWp == nullptr)
   {
      loadWaypoint();
      return;
   }

   double az = Earth::headingBetween(globals->lat, globals->lon, currentWp->radLat, currentWp->radLon);
   double dist = Earth::distance(globals->lat, globals->lon, currentWp->radLat, currentWp->radLon);

   //double hdgDiff = fabs( UnitConvert :: wrapHeading(globals->eulers.getPsi() - az, true) );
   double hdg = std::atan2(globals->nedVel.get2(), globals->nedVel.get1());
   double hdgDiff = std::fabs(UnitConvert::wrapHeading(hdg - az, true));

   bool isClose = dist < distTol;
   bool isBehind = std::fabs(hdgDiff) > sf::PI / 2.;

   if (isClose && isBehind)
   {
      loadWaypoint();
      return;
   }

   if (cmdPathType == PathType::DIRECT)
   {
      globals->autoPilotCmds.setCmdHeading(az);
   }
   else
   {
      if (hdgDiff >= PI)
      {
         globals->autoPilotCmds.setCmdHeading(az);
      }
      else
      {
         globals->autoPilotCmds.setCmdHeading(UnitConvert::wrapHeading(2 * az - currentWp->radHeading - 0.01, true));
      }
   }
}

void WaypointFollower::loadWaypoint()
{
   currentWp = nullptr;

   std::cout << "loading wpt: " << wpNum << std::endl;

   if (waypoints.size() > wpNum)
   {
      setState(true);
      currentWp = &waypoints[wpNum];
      globals->autoPilotCmds.setCmdAltitude(currentWp->meterAlt);
      globals->autoPilotCmds.setCmdSpeed(currentWp->mpsSpeed);

      // set the distance tolerance to x seconds of flight time
      distTol = (currentWp->mpsSpeed * 5) * Earth::metersToRadian;
      std::cerr << distTol << std::endl;

      wpNum++;
   }
   else
   {
      setState(false);
   }
}

void WaypointFollower::addWaypoint(double radLat, double radLon, double meterAlt, double mpsSpeed, double radHeading)
{
   Waypoint wp;
   wp.radLat = radLat;
   wp.radLon = radLon;
   wp.meterAlt = meterAlt;
   wp.mpsSpeed = mpsSpeed;
   wp.radHeading = radHeading;
   waypoints.push_back(wp);
   setState(true);
}

void WaypointFollower::clearAllWaypoints()
{
   waypoints.clear();
   wpNum = 0;
   currentWp = nullptr;
}

int WaypointFollower::getCurrentWp()
{
   return wpNum;
}

int WaypointFollower::getNumWaypoints()
{
   return waypoints.size();
}

void WaypointFollower::setCurrentWp(int num)
{
   this->wpNum = num;
}
}
