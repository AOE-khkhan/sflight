
#ifndef __sflight_mdls_Euler_HPP__
#define __sflight_mdls_Euler_HPP__

#include "sflight/mdls/Vector3.hpp"

namespace sflight {
namespace mdls {

//------------------------------------------------------------------------------
// Class: Euler
//------------------------------------------------------------------------------
class Euler : public Vector3
{
public:
    Euler() = default;
    Euler(double psi, double theta, double phi);
    virtual ~Euler() = default;

    void getUVW(Vector3 &uvw, Vector3 &dxdydz);
    void getDxDyDz(Vector3 &dxdydz, Vector3 &uvw);
    void getDeltaEuler(Euler &deltaEuler, double p, double q, double r );
    void getPQR(Vector3 &pqr, Vector3 &eulerDot);

    double getPsi()                     { return a1;  }
    void setPsi(const double x)         { a1 = x;     }
    double getTheta()                   { return a2;  }
    void setTheta(const double x)       { a2 = x;     }
    double getPhi()                     { return a3;  }
    void setPhi(const double x)         { a3 = x;     }
};

}
}

#endif
