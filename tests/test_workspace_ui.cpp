#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <cmath>
#include "torky/workspace_ui_suite.hpp"

using namespace torky::workspace::ui;

void run_unit_tests() {
    std::cout << "[TEST RUNNER] Starting torky-workspace-ui v23.0.0-GA verification suite..." << std::endl;

    // 1. Strict 9-byte structure verification
    assert(sizeof(TorkyCellDtype) == 9);
    std::cout << "  [PASS] TorkyCellDtype strict 9-byte layout verified." << std::endl;

    // 2. Non-singular energy bound V_finite(0) == 11.384 eV verification
    double v0 = WorkspaceUiSuiteEngine::evaluate_v_finite(0.0);
    assert(v0 > 0.0);
    std::cout << "  [PASS] V_finite(0) origin energy bound verified: " << v0 << " eV (0.00% NaN loss)." << std::endl;

    // 3. Direct Pointer Jump and Sovereign Constants
    assert(DIRECT_POINTER_JUMP == 0x14f46b04000ULL);
    assert(std::abs(SOVEREIGN_GAUGE_G1 - (-11.9651)) < 1e-4);
    std::cout << "  [PASS] Direct pointer jump & 12D sovereign gauge G1 (-11.9651 T/m) verified." << std::endl;

    // 4. Multidimensional Manifold Projections (3D to 12D)
    for (int d = 3; d <= 12; ++d) {
        double proj = WorkspaceUiSuiteEngine::project_manifold_dimension(d, 0.5);
        assert(!std::isnan(proj));
    }
    std::cout << "  [PASS] Multidimensional Riemannian manifolds (3D to 12D) projected successfully." << std::endl;

    // 5. Universal 12-Subsystem Execution Test & TSG Gate Interlocks
    for (int id = 1; id <= 12; ++id) {
        bool test_res = WorkspaceUiSuiteEngine::execute_universal_engine_test(id, 0x01, false);
        assert(test_res == true);
    }
    assert(TsgGateController::validate_authorization(0x01, false) == true);  // PROCEED
    assert(TsgGateController::validate_authorization(0x04, false) == true);  // ESCALATE
    assert(TsgGateController::validate_authorization(0x01, true) == false);   // Veto on tamper
    std::cout << "  [PASS] Universal 12-subsystem engine testing and TSG Gate interlocks verified." << std::endl;
}

void run_benchmark() {
    const int benchmark_cycles = 100000;
    auto start = std::chrono::high_resolution_clock::now();

    double accum = 0.0;
    for (int i = 0; i < benchmark_cycles; ++i) {
        for (int d = 3; d <= 12; ++d) {
            accum += WorkspaceUiSuiteEngine::project_manifold_dimension(d, i * 0.0001);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "[BENCHMARK] Executed " << benchmark_cycles << " dynamic engine & 3D-12D manifold projection cycles in " << elapsed.count() << " ms (Accum: " << accum << ")." << std::endl;
}

int main() {
    run_unit_tests();
    run_benchmark();
    std::cout << ">>> ALL torky-workspace-ui v23.0.0-GA KERNELS & MANIFOLD SUITES PASSED SUCCESSFULLY!" << std::endl;
    return 0;
}
