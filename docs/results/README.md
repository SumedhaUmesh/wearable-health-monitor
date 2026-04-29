# Results artifacts

Generated plots and benchmark tables for portfolio storytelling live here.

Generate a simple preview from the checked-in sample CSV:

```bash
pip install matplotlib
python3 tools/plot_results.py --ppg data/ppg_sample.csv --out docs/results/ppg_preview.png
```

Add dataset-scale accuracy tables here after running offline evaluation against PhysioNet / WISDM exports (see root README **Results**).
