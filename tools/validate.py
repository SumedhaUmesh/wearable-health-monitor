#!/usr/bin/env python3
"""
Lightweight offline check: read IMU CSV, compute sliding magnitude variance,
and report labels using the same thresholds as C++ ActivityClassifier defaults.
"""

from __future__ import annotations

import argparse
import csv
import math
from pathlib import Path


def magnitude(ax: float, ay: float, az: float) -> float:
    return math.sqrt(ax * ax + ay * ay + az * az)


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument(
        "--imu",
        type=Path,
        default=Path(__file__).resolve().parent.parent / "data" / "imu_sample.csv",
    )
    p.add_argument("--window", type=int, default=100)
    p.add_argument("--still-max", type=float, default=800.0)
    p.add_argument("--walk-max", type=float, default=8000.0)
    args = p.parse_args()

    rows: list[tuple[float, float, float]] = []
    with args.imu.open(newline="") as f:
        r = csv.DictReader(f)
        for row in r:
            rows.append(
                (float(row["ax"]), float(row["ay"]), float(row["az"]))
            )

    mags = [magnitude(*t) for t in rows]
    if len(mags) < args.window:
        print("Not enough rows for one window.")
        return

    labels = []
    for i in range(args.window - 1, len(mags)):
        w = mags[i - args.window + 1 : i + 1]
        mean = sum(w) / len(w)
        v = sum((x - mean) ** 2 for x in w) / len(w)
        if v < args.still_max:
            labels.append("still")
        elif v < args.walk_max:
            labels.append("walk")
        else:
            labels.append("run")

    print(f"windows: {len(labels)} (from {args.imu})")
    print("last label:", labels[-1] if labels else "n/a")


if __name__ == "__main__":
    main()
