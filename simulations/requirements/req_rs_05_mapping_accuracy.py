"""Synthetic mapping-error scoring harness for REQ-RS-05."""

from __future__ import annotations

import csv
import json
import math
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


DISCLAIMER = "SIMULATION - NOT BENCH EVIDENCE"
PHYSICAL_STATUS = "OPEN - physical rover test not performed"
ERROR_LIMIT_CM = 10.0


def reference_wall_points() -> np.ndarray:
    """Return eight fixed reference points, two on each wall of a 4 m room."""
    return np.array(
        [
            [0.0, 1.0], [0.0, 3.0],
            [1.0, 0.0], [3.0, 0.0],
            [4.0, 1.0], [4.0, 3.0],
            [1.0, 4.0], [3.0, 4.0],
        ],
        dtype=float,
    )


def deterministic_offsets_cm() -> list[np.ndarray]:
    """Four passing offset sets and one deliberately failing offset set."""
    x_offsets = [
        [2, -3, 1, -4, 3, -2, 4, -1],
        [4, -5, 6, -3, 4, -6, 5, -4],
        [7, -6, 5, -8, 7, -5, 6, -4],
        [9, -8, 7, -6, 5, -4, 8, -7],
        [4, -5, 12, -3, 4, -4, 5, -2],
    ]
    y_offsets = [
        [1, 2, -2, 1, -1, 3, -2, 2],
        [2, 3, -2, 4, -4, 2, -3, 4],
        [2, 4, -6, 3, -2, 5, -4, 6],
        [1, 3, -4, 5, -6, 7, -2, 4],
        [2, 3, 0, 4, -3, 2, -4, 3],
    ]
    return [np.column_stack((x, y)).astype(float) for x, y in zip(x_offsets, y_offsets)]


def point_errors_cm(reference: np.ndarray, mapped: np.ndarray) -> np.ndarray:
    if reference.shape != mapped.shape:
        raise ValueError("Reference and mapped arrays must have the same shape")
    if reference.shape != (8, 2):
        raise ValueError("Each run must contain exactly eight two-dimensional points")
    if not np.isfinite(reference).all() or not np.isfinite(mapped).all():
        raise ValueError("All coordinates must be finite")
    return 100.0 * np.linalg.norm(mapped - reference, axis=1)


def run_passes(errors_cm: np.ndarray, limit_cm: float = ERROR_LIMIT_CM) -> bool:
    if errors_cm.shape != (8,):
        raise ValueError("Each run must contain exactly eight point errors")
    return bool(np.all(errors_cm <= limit_cm + 1e-9))


def overall_passes(run_results: list[bool]) -> bool:
    if len(run_results) != 5:
        raise ValueError("Exactly five runs are required")
    return sum(run_results) >= 4


def simulate(output_dir: Path) -> dict[str, object]:
    output_dir.mkdir(parents=True, exist_ok=True)
    reference = reference_wall_points()
    offsets_cm = deterministic_offsets_cm()
    mapped_runs = [reference + offsets / 100.0 for offsets in offsets_cm]

    point_rows: list[dict[str, object]] = []
    summary_rows: list[dict[str, object]] = []
    run_results: list[bool] = []

    for run_index, mapped in enumerate(mapped_runs, start=1):
        errors = point_errors_cm(reference, mapped)
        passed = run_passes(errors)
        run_results.append(passed)
        summary_rows.append(
            {
                "run": run_index,
                "maximum_error_cm": round(float(errors.max()), 3),
                "all_eight_within_10_cm": passed,
            }
        )
        for point_index, (truth, estimate, error) in enumerate(
            zip(reference, mapped, errors), start=1
        ):
            point_rows.append(
                {
                    "run": run_index,
                    "point": point_index,
                    "reference_x_m": truth[0],
                    "reference_y_m": truth[1],
                    "synthetic_mapped_x_m": round(float(estimate[0]), 4),
                    "synthetic_mapped_y_m": round(float(estimate[1]), 4),
                    "error_cm": round(float(error), 3),
                    "within_10_cm": error <= ERROR_LIMIT_CM + 1e-9,
                }
            )

    point_csv_path = output_dir / "sim_req_rs_05_point_errors.csv"
    with point_csv_path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(point_rows[0].keys()))
        writer.writeheader()
        writer.writerows(point_rows)

    summary_csv_path = output_dir / "sim_req_rs_05_run_summary.csv"
    with summary_csv_path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(summary_rows[0].keys()))
        writer.writeheader()
        writer.writerows(summary_rows)

    fig, axes = plt.subplots(1, 5, figsize=(17, 4.8), sharex=True, sharey=True)
    for run_index, (axis, mapped, summary) in enumerate(
        zip(axes, mapped_runs, summary_rows), start=1
    ):
        axis.plot([0, 4, 4, 0, 0], [0, 0, 4, 4, 0], color="black", linewidth=1)
        axis.scatter(reference[:, 0], reference[:, 1], marker="x", s=55,
                     color="black", label="reference")
        colour = "#27ae60" if summary["all_eight_within_10_cm"] else "#c0392b"
        axis.scatter(mapped[:, 0], mapped[:, 1], s=40, color=colour,
                     label="synthetic map")
        for truth, estimate in zip(reference, mapped):
            axis.plot([truth[0], estimate[0]], [truth[1], estimate[1]],
                      color=colour, linewidth=0.8, alpha=0.8)
        status = "PASS" if summary["all_eight_within_10_cm"] else "FAIL"
        axis.set_title(
            f"Run {run_index}: {status}\nmax {summary['maximum_error_cm']:.2f} cm"
        )
        axis.set_xlim(-0.2, 4.2)
        axis.set_ylim(-0.2, 4.2)
        axis.set_aspect("equal", adjustable="box")
        axis.grid(True, linestyle=":", alpha=0.45)
        axis.set_xlabel("x (m)")
    axes[0].set_ylabel("y (m)")
    axes[0].legend(loc="center", fontsize=8)
    passing_runs = sum(run_results)
    fig.suptitle(
        f"REQ-RS-05 synthetic scoring: {passing_runs}/5 runs pass the +/-10 cm rule",
        fontsize=13,
    )
    fig.text(0.5, 0.025, f"{DISCLAIMER} | Physical requirement: {PHYSICAL_STATUS}",
             ha="center", color="darkred", fontsize=9, weight="bold")
    fig.tight_layout(rect=(0, 0.13, 1, 0.9))
    plot_path = output_dir / "sim_req_rs_05_mapping.png"
    fig.savefig(plot_path, dpi=180)
    plt.close(fig)

    software_pass = overall_passes(run_results)
    result = {
        "requirement": "REQ-RS-05",
        "simulation_check": "PASS" if software_pass else "FAIL",
        "physical_requirement": PHYSICAL_STATUS,
        "evidence_class": "synthetic acceptance-scoring test",
        "passing_runs": passing_runs,
        "required_passing_runs": 4,
        "run_maximum_errors_cm": [row["maximum_error_cm"] for row in summary_rows],
        "outputs": [point_csv_path.name, summary_csv_path.name, plot_path.name],
        "statement": (
            "The scoring software classified four of five synthetic runs as passing; "
            "no physical wall, range sensor, odometry, or rover run was tested."
        ),
    }
    json_path = output_dir / "sim_req_rs_05_result.json"
    json_path.write_text(json.dumps(result, indent=2, sort_keys=True), encoding="utf-8")
    result["outputs"].append(json_path.name)
    return result


if __name__ == "__main__":
    result = simulate(Path(__file__).resolve().parent / "outputs")
    print(json.dumps(result, indent=2))
