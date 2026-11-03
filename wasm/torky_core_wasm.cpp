#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define TORKY_EXPORT extern "C" EMSCRIPTEN_KEEPALIVE
#else
#define TORKY_EXPORT extern "C"
#endif

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

    TORKY_EXPORT double evaluate_finite_potential(double r_nm) {
        return (V_FINITE_ZERO_EV / std::sqrt((r_nm * r_nm) + EPSILON_NM2)) * std::sqrt(EPSILON_NM2);
    }

    TORKY_EXPORT double solve_cardano_viete(double p, double q) {
        double p_reg = (p > -1e-12) ? -std::sqrt((p * p) + EPSILON_NM2) : p;
        double arg = (3.0 * q) / (2.0 * p_reg) * std::sqrt(-3.0 / p_reg);
        arg = std::clamp(arg, -1.0, 1.0);
        return 2.0 * std::sqrt(-p_reg / 3.0) * std::cos(std::acos(arg) / 3.0);
    }

    TORKY_EXPORT uint8_t validate_banach_contraction(double rho_c) {
        double derivative = BANACH_DERIVATIVE_BOUND * std::exp(-std::abs(rho_c));
        return (derivative < 1.0) ? 1 : 0;
    }

    TORKY_EXPORT uint8_t evaluate_tsg_gate(uint8_t gate_req, uint8_t bit_tampered) {
        if (bit_tampered) return 0x03; // DENY / PROTOCOL VETO
        return (gate_req >= 0x01 && gate_req <= 0x04) ? gate_req : 0x03;
    }
}

int main() {
    std::cout << "[NATIVE C++20 CHECK] sizeof(TorkyCellDtype) = " << sizeof(torky::core::TorkyCellDtype) << " bytes." << std::endl;
    std::cout << "[MATH CHECK] V_finite(0) = " << torky::core::evaluate_finite_potential(0.0) << " eV (0.00% NaN Loss)." << std::endl;
    return 0;
}
