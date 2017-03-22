
#ifndef _StickControl_H
#define _StickControl_H

#include "sf/fdm/modules/FDMModule.hpp"

#include "sf/fdm/Vector3.hpp"

#include "sf/xml/Node.hpp"

namespace sf {
namespace xml {
class Node;
}
namespace fdm {

//------------------------------------------------------------------------------
// Class: StickControl
//------------------------------------------------------------------------------
class StickControl : public virtual FDMModule
{
 public:
   StickControl(FDMGlobals* globals, const double frameRate);
   virtual ~StickControl() = default;

   // module interface
   virtual void initialize(xml::Node* node) override;
   virtual void update(const double timestep) override;

 private:
   Vector3 maxRates;
   Vector3 maxDef;

   double designQbar{};
   double elevGain{};
   double rudGain{};
   double ailGain{};
   double pitchGain{};
};
}
}

#endif
