#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define TORKY_EXPORT extern "C" EMSCRIPTEN_KEEPALIVE
#else
#define TORKY_EXPORT extern "C"
#endif

namespace torky::sovereign {
    inline constexpr double V_FINITE_EV_ZERO      = 11.384;
    inline constexpr double V_FINITE_NONDIM_ZERO  = 7.905694150420948;
    inline constexpr double SCALE_FLOOR_LA_NM     = 0.36787944117144233;
    inline constexpr double BANACH_CONTRACTION_RC = 0.3849;
    inline constexpr double RIEMANN_GRADIENT_G1   = -11.9651;
    inline constexpr uint64_t SHM_BASE_PTR        = 0x14f46b04000ULL;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint64_t state;
        uint8_t  tsg_gate; // 0x01 PROCEED, 0x02 HOLD, 0x03 DENY, 0x04 ESCALATE
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "Strict 9-byte ABI alignment violated!");
}

using namespace torky::sovereign;

TORKY_EXPORT int torky_verify_abi_sizeof() {
    return static_cast<int>(sizeof(TorkyCellDtype));
}

TORKY_EXPORT double torky_compute_v_finite(double r, bool in_ev) {
    if (!std::isfinite(r)) return in_ev ? V_FINITE_EV_ZERO : V_FINITE_NONDIM_ZERO;
    const double v_nondim = 1.0 / std::sqrt((r * r) + 0.016);
    return in_ev ? (v_nondim * (V_FINITE_EV_ZERO / V_FINITE_NONDIM_ZERO)) : v_nondim;
}

TORKY_EXPORT double torky_cardano_solve_real_root(double p, double q) {
    if (!std::isfinite(p) || !std::isfinite(q)) return 0.0;
    const double half_q = q / 2.0;
    const double third_p = p / 3.0;
    const double disc = (half_q * half_q) + (third_p * third_p * third_p);
    if (disc >= 0.0) {
        const double sq = std::sqrt(disc);
        return std::cbrt(-half_q + sq) + std::cbrt(-half_q - sq);
    } else {
        const double r = std::sqrt(-third_p);
        const double arg = std::clamp(-half_q / (r * r * r), -1.0, 1.0);
        return 2.0 * r * std::cos(std::acos(arg) / 3.0);
    }
}

int main() {
    assert(torky_verify_abi_sizeof() == 9);
    double v0 = torky_compute_v_finite(0.0, true);
    double root = torky_cardano_solve_real_root(-3.0, 2.0);
    std::cout << "[SUCCESS] sizeof(TorkyCellDtype)=" << torky_verify_abi_sizeof()
              << " | V_finite(0)=" << v0 << " eV | Cardano Root(p=-3,q=2)=" << root << std::endl;
    return 0;
}
