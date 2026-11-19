"""Run all three deterministic requirement simulations."""

from __future__ import annotations

import json
from pathlib import Path

from req_rs_03_map_records import simulate as simulate_req_rs_03
from req_rs_04_anomaly_threshold import simulate as simulate_req_rs_04
from req_rs_05_mapping_accuracy import simulate as simulate_req_rs_05


DISCLAIMER = "SIMULATION - NOT BENCH EVIDENCE"


def main() -> int:
    output_dir = Path(__file__).resolve().parent / "outputs"
    results = [
        simulate_req_rs_03(output_dir),
        simulate_req_rs_04(output_dir),
        simulate_req_rs_05(output_dir),
    ]
    package_result = {
        "notice": DISCLAIMER,
        "purpose": "Verify software data flow, calculations, and pass/fail logic",
        "results": {result["requirement"]: result for result in results},
    }
    results_path = output_dir / "results.json"
    results_path.write_text(
        json.dumps(package_result, indent=2, sort_keys=True), encoding="utf-8"
    )

    print(DISCLAIMER)
    for result in results:
        print(
            f"{result['requirement']}: software check {result['simulation_check']}; "
            f"physical status {result['physical_requirement']}"
        )
    print(f"Combined results: {results_path}")
    return 0 if all(result["simulation_check"] == "PASS" for result in results) else 1


if __name__ == "__main__":
    raise SystemExit(main())
