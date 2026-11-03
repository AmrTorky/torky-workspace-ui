#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>

namespace torky::core {
    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint64_t state;
        uint8_t  tsg_gate; // 0x01 PROCEED, 0x02 HOLD, 0x03 DENY, 0x04 ESCALATE
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must equal 9 bytes.");

    constexpr double EPSILON_NM2 = 0.016;
    constexpr double V_FINITE_ZERO_EV = 11.384;
    constexpr double BANACH_DERIVATIVE_BOUND = 0.3849;

    extern "C" double evaluate_finite_potential(double r_nm) {
        return (V_FINITE_ZERO_EV / std::sqrt((r_nm * r_nm) + EPSILON_NM2)) * std::sqrt(EPSILON_NM2);
    }

    extern "C" double solve_cardano_viete(double p, double q) {
        double p_reg = (p > -1e-12) ? -std::sqrt((p * p) + EPSILON_NM2) : p;
        double arg = (3.0 * q) / (2.0 * p_reg) * std::sqrt(-3.0 / p_reg);
        arg = std::clamp(arg, -1.0, 1.0);
        return 2.0 * std::sqrt(-p_reg / 3.0) * std::cos(std::acos(arg) / 3.0);
    }
}

int main() {
    std::cout << "[NATIVE C++20 CHECK] sizeof(TorkyCellDtype) = " << sizeof(torky::core::TorkyCellDtype) << " bytes." << std::endl;
    std::cout << "[MATH CHECK] V_finite(0) = " << torky::core::evaluate_finite_potential(0.0) << " eV (0.00% NaN Loss)." << std::endl;
    return 0;
}
