#!/usr/bin/env python3
"""
csv_to_gif.py
Generates animated GIFs of the microtubule state from numbered CSVs.

Supports two output formats:
  --format cuda  (default) : reads from results_dir/animations/anim_a*/microtubule_*.csv
  --format mpi             : reads from results_dir/exp00000_*/microtubule_*.csv
                             filtering only extreme configurations (params at min/max)

Usage:
  python csv_to_gif.py --results_dir RESLT_CUDA [--delay 100] [--cmap binary]
  python csv_to_gif.py --results_dir RESLT_MPI  --format mpi [--delay 100]

Arguments:
  --results_dir  Results directory
  --format       'cuda' or 'mpi' (default: cuda)
  --delay        Duration of each frame in ms (default: 100)
  --cmap         Matplotlib colormap (default: binary)
  --dpi          Image resolution (default: 80)
"""

import os
import re
import sys
import glob
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# ──────────────────────────────────────────────────────────────────────────────
def make_gif(anim_folder: str, gif_path: str, delay_ms: int, cmap: str, dpi: int,
             title_override: str = ""):
    """Reads microtubule_NNNNN.csv files from anim_folder and generates a GIF."""
    pattern = os.path.join(anim_folder, "microtubule_*.csv")
    files   = sorted(glob.glob(pattern))

    if not files:
        print(f"  [SKIP] No CSV files in: {anim_folder}")
        return

    frames = []
    for f in files:
        try:
            m = np.loadtxt(f, delimiter=",")
            if m.ndim == 1:
                m = m[np.newaxis, :]
            frames.append(m)
        except Exception as e:
            print(f"  [WARN] Could not read {f}: {e}")

    if not frames:
        print(f"  [SKIP] No valid data in: {anim_folder}")
        return

    N, L = frames[0].shape
    folder_name = os.path.basename(anim_folder)
    title_base  = title_override if title_override else folder_name.replace("_", "  ")

    fig, ax = plt.subplots(figsize=(max(6, L // 10), max(2, N // 2 + 1)), dpi=dpi)
    im = ax.imshow(frames[0], interpolation='none', cmap=cmap,
                   vmin=0, vmax=1, aspect='auto')
    ax.set_xlabel("Position $(x)$")
    ax.set_ylabel("Channel")
    step_text = ax.set_title(f"{title_base}\nStep 0 / {len(frames)-1}")
    ax.set_xticks(np.arange(0, L, max(1, L // 10)))
    ax.yaxis.set_ticklabels([])
    plt.tight_layout()

    def update(idx):
        im.set_data(frames[idx])
        step_text.set_text(f"{title_base}\nStep {idx} / {len(frames)-1}")
        return [im, step_text]

    ani = animation.FuncAnimation(fig, update, frames=len(frames),
                                  interval=delay_ms, blit=True)
    writer = animation.PillowWriter(fps=max(1, 1000 // delay_ms))
    ani.save(gif_path, writer=writer)
    plt.close(fig)
    print(f"  [OK]  {gif_path}  ({len(frames)} frames, {N}×{L})")

# ──────────────────────────────────────────────────────────────────────────────
# Extreme parameters: parse a and b from MPI folder name:
#   exp00000_a0_b1_r1_oi0_oo1  →  {a:0, b:1, r:1, oi:0, oo:1}
PARAM_RE = re.compile(
    r"_a(?P<a>[0-9.]+)_b(?P<b>[0-9.]+)_r(?P<r>[0-9.]+)"
    r"_oi(?P<oi>[0-9.]+)_oo(?P<oo>[0-9.]+)"
)

def parse_params(folder_name: str):
    """Extracts floating point parameters from the folder name. Returns dict or None."""
    m = PARAM_RE.search(folder_name)
    if not m:
        return None
    return {k: float(v) for k, v in m.groupdict().items()}

def is_extreme(val: float, lo: float, hi: float, tol: float = 1e-9) -> bool:
    return abs(val - lo) < tol or abs(val - hi) < tol

def find_extreme_mpi_folders(results_dir: str):
    """
    Returns the MPI folders (exp00000_*) corresponding to extreme configurations,
    meaning all its parameters are at their minimum or maximum sweep value.
    """
    # Only experiment 0 (sufficient to visualize the dynamics)
    all_folders = sorted(glob.glob(os.path.join(results_dir, "exp00000_*")))
    if not all_folders:
        return []

    # Collect all unique values for each parameter
    param_vals = {k: set() for k in ('a', 'b', 'r', 'oi', 'oo')}
    for folder in all_folders:
        p = parse_params(os.path.basename(folder))
        if p:
            for k in param_vals:
                param_vals[k].add(p[k])

    # Extremes = min and max of each parameter
    extremes = {k: (min(vs), max(vs)) for k, vs in param_vals.items()}

    extreme_folders = []
    for folder in all_folders:
        p = parse_params(os.path.basename(folder))
        if p and all(is_extreme(p[k], *extremes[k]) for k in ('a', 'b', 'r', 'oi', 'oo')):
            extreme_folders.append(folder)

    return extreme_folders

# ──────────────────────────────────────────────────────────────────────────────
def main():
    parser = argparse.ArgumentParser(
        description="Generates animated GIFs of the microtubule state")
    parser.add_argument("--results_dir", required=True,
                        help="Results directory")
    parser.add_argument("--format", choices=["cuda", "mpi"], default="cuda",
                        help="Output format: 'cuda' (default) or 'mpi'")
    parser.add_argument("--delay", type=int, default=100,
                        help="Duration of each frame in ms (default: 100)")
    parser.add_argument("--cmap", default="binary",
                        help="Matplotlib colormap (default: binary)")
    parser.add_argument("--dpi", type=int, default=80,
                        help="Image resolution (default: 80)")
    args = parser.parse_args()

    results_dir = os.path.abspath(args.results_dir)
    if not os.path.isdir(results_dir):
        print(f"ERROR: Directory does not exist: {results_dir}")
        sys.exit(1)

    gifs_dir = os.path.join(results_dir, "gifs")
    os.makedirs(gifs_dir, exist_ok=True)
    print(f"Format: {args.format}")
    print(f"GIFs → {gifs_dir}\n")

    # ── CUDA Mode ──────────────────────────────────────────────────────────────
    if args.format == "cuda":
        animations_dir = os.path.join(results_dir, "animations")
        if not os.path.isdir(animations_dir):
            print(f"ERROR: 'animations/' does not exist in {results_dir}")
            print("Run the simulation with --output_microtubule_state 1 first.")
            sys.exit(1)

        anim_folders = sorted(glob.glob(os.path.join(animations_dir, "anim_a*")))
        if not anim_folders:
            print("No se encontraron carpetas 'anim_a*'.")
            sys.exit(0)

        print(f"Found {len(anim_folders)} extreme configurations (CUDA).\n")

        for folder in anim_folders:
            folder_name = os.path.basename(folder)
            gif_name    = folder_name.replace("anim_", "") + ".gif"
            gif_path    = os.path.join(gifs_dir, gif_name)
            title       = folder_name.replace("anim_", "").replace("_", "  ")
            print(f"Processing: {folder_name}")
            make_gif(folder, gif_path, args.delay, args.cmap, args.dpi, title)

    # ── MPI Mode ───────────────────────────────────────────────────────────────
    else:
        extreme_folders = find_extreme_mpi_folders(results_dir)
        if not extreme_folders:
            print("No 'exp00000_*' folders with extreme configurations found.")
            print("Run the MPI simulation with --output_microtubule_state 1 first.")
            sys.exit(0)

        print(f"Found {len(extreme_folders)} extreme configurations (MPI).\n")

        for folder in extreme_folders:
            folder_name = os.path.basename(folder)
            # GIF name: remove the "exp00000_" prefix
            gif_name = folder_name.replace("exp00000_", "") + ".gif"
            gif_path = os.path.join(gifs_dir, gif_name)
            # Clean title for the GIF
            p = parse_params(folder_name)
            title = (f"α={p['a']}  β={p['b']}  ρ={p['r']}  ωin={p['oi']}  ωout={p['oo']}"
                     if p else folder_name)
            print(f"Processing: {folder_name}")
            make_gif(folder, gif_path, args.delay, args.cmap, args.dpi, title)

    print(f"\nDone. GIFs saved in: {gifs_dir}")

if __name__ == "__main__":
    main()
