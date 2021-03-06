
#include "sflight/mdls/Euler.hpp"

#include "sflight/mdls/Vector3.hpp"

#include <cmath>

namespace sflight {
namespace mdls {

Euler::Euler(double psi, double theta, double phi)
{
    Vector3();
    this->a1 = psi;
    this->a2 = theta;
    this->a3 = phi;
}

void Euler::getDxDyDz(Vector3 &dxdydz, Vector3 &uvw)
{
    double a11{std::cos(a2) * std::cos(a1)};
    double a12{std::sin(a3) * std::sin(a2) * std::cos(a1) - std::cos(a3) * std::sin(a1)};
    double a13{std::cos(a3) * std::sin(a2) * std::cos(a1) + std::sin(a3) * std::sin(a1)};
    double a21{std::cos(a2) * std::sin(a1)};
    double a22{std::sin(a3) * std::sin(a2) * std::sin(a1) + std::cos(a3) * std::cos(a1)};
    double a23{std::cos(a3) * std::sin(a2) * std::sin(a1) - std::sin(a3) * std::cos(a1)};
    double a31{-std::sin(a2)};
    double a32{std::sin(a3) * std::cos(a2)};
    double a33{std::cos(a3) * std::cos(a2)};

    dxdydz.set1(a11 * uvw.get1() + a12 * uvw.get2() + a13 * uvw.get3());
    dxdydz.set2(a21 * uvw.get1() + a22 * uvw.get2() + a23 * uvw.get3());
    dxdydz.set3(a31 * uvw.get1() + a32 * uvw.get2() + a33 * uvw.get3());
}

void Euler::getUVW(Vector3 &uvw, Vector3 &dxdydz)
{
    double a11{std::cos(a2) * std::cos(a1)};
    double a21{std::sin(a3) * std::sin(a2) * std::cos(a1) - std::cos(a3) * std::sin(a1)};
    double a31{std::cos(a3) * std::sin(a2) * std::cos(a1) + std::sin(a3) * std::sin(a1)};
    double a12{std::cos(a2) * std::sin(a1)};
    double a22{std::sin(a3) * std::sin(a2) * std::sin(a1) + std::cos(a3) * std::cos(a1)};
    double a32{std::cos(a3) * std::sin(a2) * std::sin(a1) - std::sin(a3) * std::cos(a1)};
    double a13{-std::sin(a2)};
    double a23{std::sin(a3) * std::cos(a2)};
    double a33{std::cos(a3) * std::cos(a2)};

    uvw.set1(a11 * dxdydz.get1() + a12 * dxdydz.get2() + a13 * dxdydz.get3());
    uvw.set2(a21 * dxdydz.get1() + a22 * dxdydz.get2() + a23 * dxdydz.get3());
    uvw.set3(a31 * dxdydz.get1() + a32 * dxdydz.get2() + a33 * dxdydz.get3());
}

void Euler::getDeltaEuler(Euler &deltaEuler, double p, double q, double r)
{
    deltaEuler.setPsi( std::sin(a3) / std::cos(a2) * q + std::cos(a3) / std::cos(a2) * r );
    deltaEuler.setTheta( std::cos(a3) * q - std::sin(a3) * r );
    deltaEuler.setPhi( p + std::sin(a3) * std::tan(a2) * q + std::cos(a3) * std::tan(a2) * r );

}

// returns the pqr vector for a given vector of delta [psi, theta, phi]
void Euler::getPQR(Vector3 &pqr, Vector3 &eulerDot)
{
    pqr.set1( eulerDot.get3() - eulerDot.get1() * std::sin(a2) );
    pqr.set2( a2 * std::cos(a3) + eulerDot.get1() * std::cos(a2) * std::sin(a3) );
    pqr.set3( -eulerDot.get2() * std::sin(a3) + eulerDot.get1() * std::cos(a2) * std::cos(a3) );
}

}
}
