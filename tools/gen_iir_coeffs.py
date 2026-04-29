#!/usr/bin/env python3
"""Regenerate SOS rows for src/algorithms/heart_rate_coeffs.h (requires scipy)."""

from __future__ import annotations

import argparse

import numpy as np
from scipy.signal import butter


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--fs", type=float, default=25.0)
    p.add_argument("--low", type=float, default=0.5)
    p.add_argument("--high", type=float, default=4.0)
    p.add_argument("--order", type=int, default=2)
    args = p.parse_args()
    sos = butter(
        args.order,
        [args.low, args.high],
        btype="band",
        fs=args.fs,
        output="sos",
    )
    np.set_printoptions(precision=16, floatmode="fixed")
    print(f"// fs={args.fs} Hz band [{args.low}, {args.high}] Hz, order={args.order}")
    print("static constexpr double kSos[][6] = {")
    for row in sos:
        parts = ", ".join(f"{float(x):.16f}" for x in row)
        print(f"    {{{parts}}},")
    print("};")


if __name__ == "__main__":
    main()
