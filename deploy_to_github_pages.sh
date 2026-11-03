#!/usr/bin/env bash
set -e
cp -f index.html docs/index.html
git add index.html docs/index.html wasm/torky_core_wasm.cpp deploy_to_github_pages.sh
git commit -m "feat(phase1-4): deploy C++20 WASM core, 3D-12D WebGL UI & Terminal Clone Engine (v25.0.0-GA)" || echo "No changes to commit"
git tag -f -a v25.0.0-GA -m "4-Phase Standalone Production Release v25.0.0-GA"
git push origin main
git push origin --tags -f
