#ifndef TORKY_WORKSPACE_UI_SUITE_HPP
#define TORKY_WORKSPACE_UI_SUITE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace torky::workspace::ui {
    struct SubsystemMetadata {
        std::string name;
        std::string doi;
        uint8_t tsg_status;
    };

    class WorkspaceUISuite {
    public:
        static std::vector<SubsystemMetadata> get_all_sixteen_subsystems() {
            return {
                {"torky-workspace-ui", "10.5281/zenodo.23900809", 0x01},
                {"torky-ipc-daemon", "10.5281/zenodo.23900813", 0x01},
                {"torky-core-continuum", "10.5281/zenodo.23900816", 0x01},
                {"torky-cardano-solver", "10.5281/zenodo.23900826", 0x01},
                {"torky-ast-metacompiler", "10.5281/zenodo.23900834", 0x01},
                {"torky-silicon-kernels", "10.5281/zenodo.23900849", 0x01},
                {"torky-quantum-bridge", "10.5281/zenodo.23900855", 0x01},
                {"torky-lakehouse-bridges", "10.5281/zenodo.23900861", 0x01},
                {"torky-governance-engine", "10.5281/zenodo.23900869", 0x01},
                {"torky-sovereign-ai-governor", "10.5281/zenodo.23900879", 0x01},
                {"torky-wasm-node", "10.5281/zenodo.23900887", 0x01},
                {"torky-master-fabric", "10.5281/zenodo.23900894", 0x01},
                {"torky-cosmo-plasma", "10.5281/zenodo.23900897", 0x01},
                {"torky-thermodynamic-reset", "10.5281/zenodo.23900899", 0x01},
                {"torky-topological-braid", "10.5281/zenodo.23900901", 0x01},
                {"torky-engine-appliance", "10.5281/zenodo.23900809", 0x01}
            };
        }
    };
}
#endif
