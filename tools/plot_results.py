#!/usr/bin/env python3
"""Plot sample CSV waveforms into docs/results/ for README and portfolio."""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path


def read_ppg_column(path: Path, column: str = "value") -> list[float]:
    xs: list[float] = []
    with path.open(newline="") as f:
        r = csv.DictReader(f)
        if column not in (r.fieldnames or []):
            raise SystemExit(f"Column {column!r} not in {path}")
        for row in r:
            xs.append(float(row[column]))
    return xs


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--ppg", type=Path, default=Path("data/ppg_sample.csv"))
    p.add_argument("--column", default="value")
    p.add_argument(
        "--out",
        type=Path,
        default=Path("docs/results/ppg_preview.png"),
    )
    args = p.parse_args()

    try:
        import matplotlib

        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except ImportError as e:
        print("Install matplotlib: pip install matplotlib", file=sys.stderr)
        raise SystemExit(1) from e

    samples = read_ppg_column(args.ppg, args.column)
    args.out.parent.mkdir(parents=True, exist_ok=True)

    fig, ax = plt.subplots(figsize=(8, 3))
    ax.plot(range(len(samples)), samples, color="#1f77b4", linewidth=1.2)
    ax.set_title("PPG sample replay (from CSV)")
    ax.set_xlabel("sample index")
    ax.set_ylabel("amplitude (mock units)")
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(args.out, dpi=150)
    print(f"Wrote {args.out}")


if __name__ == "__main__":
    main()
