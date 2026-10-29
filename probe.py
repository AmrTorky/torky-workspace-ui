import urllib.request, json, time, datetime, sys

USER = "amrtorky"
repos = [
    "torky-engine-appliance",
    "torky-core-continuum",
    "torky-quantum-bridge",
    "torky-cardano-solver",
    "torky-ast-metacompiler",
    "torky-silicon-kernels",
    "torky-ipc-daemon",
    "torky-lakehouse-bridges",
    "torky-hardware-simulator",
    "torky-ontology-security",
    "torky-workspace-ui",
    "torky-governance-specs"
]

timestamp = datetime.datetime.now(datetime.timezone.utc).isoformat()
report = {
    "timestamp": timestamp,
    "iso_iec_17025_cert": "1dd69a153b00",
    "total_probed": len(repos),
    "successful": 0,
    "failed": 0,
    "nodes": []
}

lines = [
    f"### Sovereign Mesh Nightly Probe --- {timestamp}",
    "",
    "| Repository | HTTP Status | Latency (ms) | Target URL |",
    "|:---|:---:|:---:|:---|"
]

has_failure = False
for r in repos:
    url = f"https://{USER}.github.io/{r}/"
    t0 = time.perf_counter()
    status_code = 0
    try:
        req = urllib.request.Request(url, headers={"User-Agent": "Torky-Mesh-Probe/1.0"})
        with urllib.request.urlopen(req, timeout=10) as resp:
            status_code = resp.status
    except Exception as e:
        status_code = getattr(e, "code", 500)

    lat = round((time.perf_counter() - t0) * 1000, 2)
    is_up = (status_code == 200)
    if is_up:
        report["successful"] += 1
    else:
        report["failed"] += 1
        has_failure = True

    report["nodes"].append({"repo": r, "status": status_code, "latency_ms": lat, "healthy": is_up})
    badge = "🟢 200 OK" if is_up else f"🔴 {status_code}"
    lines.append(f"| `{r}` | {badge} | {lat} | [{url}]({url}) |")

with open("docs/mesh_health_latest.json", "w") as f:
    json.dump(report, f, indent=2)

out = "\n".join(lines)
with open("docs/UPTIME_HISTORY.md", "a") as f:
    f.write(out + "\n\n---\n\n")

print(f"Probe finished: {report['successful']}/{report['total_probed']} healthy.")
if has_failure:
    sys.exit(1)
