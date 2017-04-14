
#ifndef __sflight_mdls_Module_H__
#define __sflight_mdls_Module_H__

#include <string>

namespace sflight {
namespace xml {
class Node;
}
namespace mdls {
class Player;

//------------------------------------------------------------------------------
// Class: Module
// Description: Base class for all modules
//------------------------------------------------------------------------------
class Module
{
 public:
   Module(Player*, const double frameRate);
   virtual ~Module() = default;

   // module interface
   virtual void initialize(xml::Node*){};
   virtual void update(const double timestep){};
   virtual void setProperty(std::string tag, const double val){};

   Player* player{};

   double frameTime{};
   double lastTime{};
};
}
}

#endif
