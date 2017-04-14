
#ifndef __SimExec_H__
#define __SimExec_H__

namespace sflight {
namespace mdls { class Player; }
namespace xml {  class Node; }
}

//------------------------------------------------------------------------------
// Class: SimExec
//------------------------------------------------------------------------------
class SimExec
{
 public:
   SimExec(sflight::mdls::Player*, const double frameRate);
   SimExec(sflight::mdls::Player*, const double frameRate, const long maxFrames);
   virtual ~SimExec() = default;

   void start();
   void startConstructive();
   void stop();
   void initialize(sflight::xml::Node* node);

 private:
   sflight::mdls::Player* player{};
   double frameRate{};
   long maxFrames{1000000000};
};

#endif
