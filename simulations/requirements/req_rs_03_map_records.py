"""Synthetic data-structure and map-rendering check for REQ-RS-03."""

from __future__ import annotations

import csv
import json
import math
from dataclasses import asdict, dataclass
from pathlib import Path

import matplotlib.pyplot as plt


DISCLAIMER = "SIMULATION - NOT BENCH EVIDENCE"
PHYSICAL_STATUS = "OPEN - physical test not performed"


@dataclass(frozen=True)
class SensorRecord:
    location_id: str
    x_m: float
    y_m: float
    temperature_c: float
    humidity_percent: float
    relative_gas_score: float


def validate_records(
    records: list[SensorRecord], room_width_m: float = 4.0, room_height_m: float = 4.0
) -> None:
    """Raise ValueError unless the synthetic records satisfy the map schema."""
    if len(records) < 3:
        raise ValueError("At least three location-tagged records are required")

    identifiers = [record.location_id for record in records]
    positions = [(record.x_m, record.y_m) for record in records]
    if len(set(identifiers)) != len(identifiers):
        raise ValueError("Location identifiers must be unique")
    if len(set(positions)) != len(positions):
        raise ValueError("Location positions must be unique")

    for record in records:
        numeric_values = (
            record.x_m,
            record.y_m,
            record.temperature_c,
            record.humidity_percent,
            record.relative_gas_score,
        )
        if not all(math.isfinite(value) for value in numeric_values):
            raise ValueError(f"{record.location_id} contains a non-finite value")
        if not (0.0 <= record.x_m <= room_width_m):
            raise ValueError(f"{record.location_id} is outside the room width")
        if not (0.0 <= record.y_m <= room_height_m):
            raise ValueError(f"{record.location_id} is outside the room height")
        if not (0.0 <= record.humidity_percent <= 100.0):
            raise ValueError(f"{record.location_id} has invalid relative humidity")


def default_records() -> list[SensorRecord]:
    return [
        SensorRecord("P1", 0.8, 1.1, 21.7, 43.0, 100.0),
        SensorRecord("P2", 2.0, 2.7, 22.1, 45.2, 112.0),
        SensorRecord("P3", 3.4, 1.8, 22.6, 47.0, 132.0),
    ]


def simulate(output_dir: Path) -> dict[str, object]:
    output_dir.mkdir(parents=True, exist_ok=True)
    records = default_records()
    validate_records(records)

    csv_path = output_dir / "sim_req_rs_03_records.csv"
    with csv_path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(asdict(records[0]).keys()))
        writer.writeheader()
        writer.writerows(asdict(record) for record in records)

    fig, axis = plt.subplots(figsize=(7.2, 6.2))
    scores = [record.relative_gas_score for record in records]
    points = axis.scatter(
        [record.x_m for record in records],
        [record.y_m for record in records],
        c=scores,
        cmap="viridis",
        vmin=90,
        vmax=140,
        s=190,
        edgecolors="black",
        zorder=3,
    )
    for record in records:
        label = (
            f"{record.location_id}\n"
            f"{record.temperature_c:.1f} C, {record.humidity_percent:.1f}% RH\n"
            f"relative score {record.relative_gas_score:.0f}"
        )
        place_left = record.x_m > 3.0
        axis.annotate(
            label,
            (record.x_m, record.y_m),
            xytext=(-8, 8) if place_left else (8, 8),
            textcoords="offset points",
            ha="right" if place_left else "left",
            fontsize=9,
        )

    axis.set_xlim(0, 4)
    axis.set_ylim(0, 4)
    axis.set_aspect("equal", adjustable="box")
    axis.set_xticks(range(5))
    axis.set_yticks(range(5))
    axis.grid(True, linestyle=":", alpha=0.6)
    axis.set_xlabel("Synthetic x position (m)")
    axis.set_ylabel("Synthetic y position (m)")
    axis.set_title("REQ-RS-03: three synthetic location-tagged records")
    fig.colorbar(points, ax=axis, label="Normalized relative-gas score")
    fig.text(0.5, 0.015, f"{DISCLAIMER} | Physical requirement: {PHYSICAL_STATUS}",
             ha="center", color="darkred", fontsize=9, weight="bold")
    fig.tight_layout(rect=(0, 0.04, 1, 1))
    plot_path = output_dir / "sim_req_rs_03_map.png"
    fig.savefig(plot_path, dpi=180)
    plt.close(fig)

    result = {
        "requirement": "REQ-RS-03",
        "simulation_check": "PASS",
        "physical_requirement": PHYSICAL_STATUS,
        "evidence_class": "synthetic software test",
        "record_count": len(records),
        "outputs": [csv_path.name, plot_path.name],
        "statement": (
            "The software represented three valid synthetic environmental records; "
            "no physical sensor or rover location was tested."
        ),
    }
    json_path = output_dir / "sim_req_rs_03_result.json"
    json_path.write_text(json.dumps(result, indent=2, sort_keys=True), encoding="utf-8")
    result["outputs"].append(json_path.name)
    return result


if __name__ == "__main__":
    result = simulate(Path(__file__).resolve().parent / "outputs")
    print(json.dumps(result, indent=2))
