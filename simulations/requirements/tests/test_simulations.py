"""Unit tests for the synthetic requirement simulation harness."""

from __future__ import annotations

import math
import sys
import tempfile
import unittest
from pathlib import Path

import numpy as np


MODULE_DIR = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(MODULE_DIR))

from req_rs_03_map_records import SensorRecord, default_records, simulate as simulate_03, validate_records
from req_rs_04_anomaly_threshold import anomaly_flag, simulate as simulate_04
from req_rs_05_mapping_accuracy import overall_passes, point_errors_cm, run_passes, simulate as simulate_05


class RequirementSimulationTests(unittest.TestCase):
    def test_req_rs_03_default_records_are_valid(self) -> None:
        validate_records(default_records())

    def test_req_rs_03_rejects_fewer_than_three_records(self) -> None:
        with self.assertRaises(ValueError):
            validate_records(default_records()[:2])

    def test_req_rs_03_rejects_duplicate_position(self) -> None:
        records = default_records()
        records[2] = SensorRecord("P3", records[0].x_m, records[0].y_m, 22, 45, 110)
        with self.assertRaises(ValueError):
            validate_records(records)

    def test_req_rs_03_rejects_non_finite_value(self) -> None:
        records = default_records()
        records[2] = SensorRecord("P3", 3.4, 1.8, math.nan, 45, 110)
        with self.assertRaises(ValueError):
            validate_records(records)

    def test_req_rs_04_boundary(self) -> None:
        self.assertFalse(anomaly_flag(100, 124.999))
        self.assertTrue(anomaly_flag(100, 125.0))
        self.assertTrue(anomaly_flag(100, 125.001))

    def test_req_rs_04_rejects_invalid_baseline(self) -> None:
        for baseline in (0, -1, math.nan):
            with self.assertRaises(ValueError):
                anomaly_flag(baseline, 125)

    def test_req_rs_05_ten_cm_passes_and_10_01_fails(self) -> None:
        reference = np.zeros((8, 2), dtype=float)
        mapped = reference.copy()
        mapped[0, 0] = 0.1
        self.assertTrue(run_passes(point_errors_cm(reference, mapped)))
        mapped[0, 0] = 0.1001
        self.assertFalse(run_passes(point_errors_cm(reference, mapped)))

    def test_req_rs_05_requires_eight_points(self) -> None:
        with self.assertRaises(ValueError):
            point_errors_cm(np.zeros((7, 2)), np.zeros((7, 2)))

    def test_req_rs_05_four_of_five_rule(self) -> None:
        self.assertTrue(overall_passes([True, True, True, True, False]))
        self.assertFalse(overall_passes([True, True, True, False, False]))
        with self.assertRaises(ValueError):
            overall_passes([True] * 4)

    def test_all_simulations_generate_outputs(self) -> None:
        # Keep temporary output inside the project so the test also works in
        # restricted Windows environments.
        with tempfile.TemporaryDirectory(dir=MODULE_DIR) as temp_dir:
            output_dir = Path(temp_dir)
            results = [simulate_03(output_dir), simulate_04(output_dir), simulate_05(output_dir)]
            self.assertTrue(all(result["simulation_check"] == "PASS" for result in results))
            self.assertTrue(all("OPEN" in result["physical_requirement"] for result in results))
            self.assertGreaterEqual(len(list(output_dir.glob("*.png"))), 3)
            self.assertGreaterEqual(len(list(output_dir.glob("*.csv"))), 4)


if __name__ == "__main__":
    unittest.main()
