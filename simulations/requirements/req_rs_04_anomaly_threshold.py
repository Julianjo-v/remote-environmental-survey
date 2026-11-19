"""Boundary-condition simulation for the REQ-RS-04 anomaly comparator."""

from __future__ import annotations

import csv
import json
import math
from pathlib import Path

import matplotlib.pyplot as plt


DISCLAIMER = "SIMULATION - NOT BENCH EVIDENCE"
PHYSICAL_STATUS = "OPEN - physical test not performed"


def anomaly_flag(baseline: float, normalized_score: float, factor: float = 1.25) -> bool:
    """Return True when the normalized score meets or exceeds the threshold."""
    if not math.isfinite(baseline) or baseline <= 0:
        raise ValueError("Baseline must be a positive finite number")
    if not math.isfinite(normalized_score):
        raise ValueError("Normalized score must be finite")
    if not math.isfinite(factor) or factor <= 0:
        raise ValueError("Threshold factor must be positive and finite")
    return normalized_score >= baseline * factor


def simulate(output_dir: Path) -> dict[str, object]:
    output_dir.mkdir(parents=True, exist_ok=True)
    baseline = 100.0
    factor = 1.25
    threshold = baseline * factor
    test_scores = [100.0, 124.0, 124.999, 125.0, 125.001, 140.0]
    rows = [
        {
            "baseline": baseline,
            "threshold_factor": factor,
            "threshold": threshold,
            "normalized_score": score,
            "flagged": anomaly_flag(baseline, score, factor),
        }
        for score in test_scores
    ]

    results_by_score = {row["normalized_score"]: row["flagged"] for row in rows}
    boundary_pass = (
        results_by_score[124.0] is False
        and results_by_score[124.999] is False
        and results_by_score[125.0] is True
        and results_by_score[125.001] is True
    )

    csv_path = output_dir / "sim_req_rs_04_threshold.csv"
    with csv_path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)

    fig, axis = plt.subplots(figsize=(8.2, 4.8))
    colours = ["#c0392b" if row["flagged"] else "#2980b9" for row in rows]
    axis.bar([str(row["normalized_score"]) for row in rows],
             [row["normalized_score"] for row in rows], color=colours)
    axis.axhline(threshold, color="black", linestyle="--",
                 label=f"threshold = {threshold:.0f}")
    axis.set_ylim(95, 145)
    axis.set_xlabel("Synthetic normalized relative-gas score")
    axis.set_ylabel("Score")
    axis.set_title("REQ-RS-04 boundary test: 124 does not flag; 125 flags")
    axis.legend()
    fig.text(0.5, 0.015, f"{DISCLAIMER} | Physical requirement: {PHYSICAL_STATUS}",
             ha="center", color="darkred", fontsize=9, weight="bold")
    fig.tight_layout(rect=(0, 0.04, 1, 1))
    plot_path = output_dir / "sim_req_rs_04_threshold.png"
    fig.savefig(plot_path, dpi=180)
    plt.close(fig)

    result = {
        "requirement": "REQ-RS-04",
        "simulation_check": "PASS" if boundary_pass else "FAIL",
        "physical_requirement": PHYSICAL_STATUS,
        "evidence_class": "synthetic software boundary test",
        "baseline": baseline,
        "threshold": threshold,
        "score_124_flagged": results_by_score[124.0],
        "score_125_flagged": results_by_score[125.0],
        "outputs": [csv_path.name, plot_path.name],
        "statement": (
            "The comparator handled the synthetic 124/125 boundary correctly; "
            "sensor baseline stability and physical air response were not tested."
        ),
    }
    json_path = output_dir / "sim_req_rs_04_result.json"
    json_path.write_text(json.dumps(result, indent=2, sort_keys=True), encoding="utf-8")
    result["outputs"].append(json_path.name)
    return result


if __name__ == "__main__":
    result = simulate(Path(__file__).resolve().parent / "outputs")
    print(json.dumps(result, indent=2))
