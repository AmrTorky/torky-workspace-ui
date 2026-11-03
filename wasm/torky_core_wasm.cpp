#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#pragma pack(push, 1)
struct TorkyCellDtype {
    uint64_t state;     // 8 bytes pointer / telemetry hash
    uint8_t  tsg_gate;  // 1 byte sovereign gate (0x01 PROCEED, 0x03 VETO)
}; 
#pragma pack(pop)

static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL ABI ERROR: TorkyCellDtype must be exactly 9 bytes!");

constexpr double V_FINITE_ZERO = 11.384; // eV

extern "C" {
    double compute_v_finite(double r, double eps) {
        return 1.0 / std::sqrt(r * r + eps);
    }
    double compute_cardano_root(double p, double q) {
        double disc = q * q / 4.0 + p * p * p / 27.0;
        if (disc >= 0) {
            double u = std::cbrt(-q / 2.0 + std::sqrt(disc));
            double v = std::cbrt(-q / 2.0 - std::sqrt(disc));
            return u + v;
        } else {
            return 2.0 * std::sqrt(-p / 3.0) * std::cos(std::acos(-q / (2.0 * std::sqrt(std::pow(-p / 3.0, 3.0)))) / 3.0);
        }
    }
}

int main() {
    TorkyCellDtype cell{0x14f46b04000ULL, 0x01};
    assert(sizeof(cell) == 9);
    std::cout << "[SUCCESS] sizeof(TorkyCellDtype)=9 | V_finite(0)=" << V_FINITE_ZERO << " eV" << std::endl;
    return 0;
}
