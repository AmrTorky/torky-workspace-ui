#ifndef TORKY_SOVEREIGN_GOVERNANCE_ENGINE_HPP
#define TORKY_SOVEREIGN_GOVERNANCE_ENGINE_HPP

#include <cmath>
#include <cstdint>
#include <array>
#include <vector>
#include <immintrin.h>

namespace torky::sovereign::engine {

    inline constexpr double L_A_NM            = 0.36787944117144233;
    inline constexpr double EPSILON_NM2       = 0.016;
    inline constexpr double V_FINITE_ZERO_EV  = 11.384;
    inline constexpr double FIM_LAMBDA_MIN    = 7.38905609893065;
    inline constexpr double HESSIAN_KAPPA_MAX = 18.50;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint8_t  gov_gate;
        int16_t  phase_q16;
        int16_t  field_q16;
        uint32_t syndrome_u32;
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must be precisely 9 bytes!");

    class NonSingularSimdKernel {
    public:
        static inline double evaluate_v_finite(double r_nm) {
            return 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
        }

        static void evaluate_avx512_batch(const double* __restrict r_in, double* __restrict v_out, size_t n) {
            __m512d eps_vec = _mm512_set1_pd(EPSILON_NM2);
            __m512d one_vec = _mm512_set1_pd(1.0);
            for (size_t i = 0; i < n; i += 8) {
                __m512d r_vec = _mm512_loadu_pd(&r_in[i]);
                __m512d r_sq  = _mm512_mul_pd(r_vec, r_vec);
                __m512d sum   = _mm512_add_pd(r_sq, eps_vec);
                __m512d root  = _mm512_sqrt_pd(sum);
                __m512d v_val = _mm512_div_pd(one_vec, root);
                _mm512_storeu_pd(&v_out[i], v_val);
            }
        }
    };

    class CardanoVieteSolver {
    public:
        static inline double solve_depressed_cubic(double p, double q) {
            double p_safe = std::min(-0.0001, p);
            double arg = (3.0 * q / (2.0 * p_safe)) * std::sqrt(-3.0 / p_safe);
            arg = std::max(-1.0, std::min(1.0, arg));
            return 2.0 * std::sqrt(-p_safe / 3.0) * std::cos(std::acos(arg) / 3.0);
        }
    };

    class CeosHardwareGateController {
    public:
        static inline bool validate_gate_authorization(uint8_t gate_request, bool bit_tampered) {
            if (bit_tampered) return false;
            return (gate_request >= 0x01 && gate_request <= 0x04);
        }
    };

} // namespace torky::sovereign::engine

#endif // TORKY_SOVEREIGN_GOVERNANCE_ENGINE_HPP
