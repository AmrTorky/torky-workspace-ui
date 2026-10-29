CXX ?= g++
CXXFLAGS = -O3 -std=c++20 -march=native -Wall -Wextra -Iinclude -pthread

TARGET = build/torky-workspace-cli
UI_BIN = build/dashboard_telemetry_engine

all: $(TARGET) $(UI_BIN)

$(TARGET): src/torky_cli_kernel.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/torky_cli_kernel.cpp -o $(TARGET)

$(UI_BIN): src/ui/dashboard_telemetry_engine.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/ui/dashboard_telemetry_engine.cpp -o $(UI_BIN)

test: all
	./$(TARGET)
	./$(UI_BIN)
	python3 tests/test_ui_endpoints.py
	./run_audit.sh

clean:
	rm -rf build local_verification_certificate.json
