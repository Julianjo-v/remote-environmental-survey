# Requirement Simulation Harness

These Python simulations verify the data structures, calculations, boundary
conditions, and acceptance-scoring logic associated with `REQ-RS-03`,
`REQ-RS-04`, and `REQ-RS-05`.

They use synthetic inputs. They are **not** sensor measurements, rover mapping
results, or physical acceptance evidence. The physical status of all three
requirements remains **OPEN** until the documented bench and rover tests are
performed.

## Simulations

- `req_rs_03_map_records.py` creates a 4 m by 4 m room map containing three
  synthetic, location-tagged temperature, humidity, and normalized relative-gas
  records.
- `req_rs_04_anomaly_threshold.py` verifies that a normalized score of `124`
  does not flag and a score of `125` does flag when the baseline is `100`.
- `req_rs_05_mapping_accuracy.py` checks eight reference wall points over five
  synthetic runs. Four runs stay within 10 cm; one deliberately fails.

## Run

From the repository root:

```powershell
python -m pip install -r simulations/requirements/requirements.txt
python simulations/requirements/run_all.py
python -m unittest discover -s simulations/requirements/tests -v
```

Generated CSV, JSON, and PNG files are written to
`simulations/requirements/outputs/`.

The simulation package passes when all software checks produce the expected
results. A software pass must not be reported as the physical requirement
passing.

## Evidence boundary

Permitted conclusion:

> The simulation harness produced the expected result with synthetic data. The
> physical requirement remains open.

Do not use these outputs to claim that a gas was detected, that physical
mapping accuracy was achieved, or that the rover passed an acceptance test.
