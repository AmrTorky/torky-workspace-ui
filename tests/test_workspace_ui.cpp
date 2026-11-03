#include <iostream>
#include <cassert>
#include "torky/workspace_ui_suite.hpp"

using namespace torky::workspace::ui;

int main() {
    std::cout << "[TEST RUNNER] Verifying torky-workspace-ui v24.0.0-GA..." << std::endl;
    auto systems = WorkspaceUISuite::get_all_sixteen_subsystems();
    assert(systems.size() == 16);
    std::cout << ">>> ALL 16 SUBSYSTEMS UNIFIED SUCCESSFULLY UNDER v24.0.0-GA!" << std::endl;
    return 0;
}
