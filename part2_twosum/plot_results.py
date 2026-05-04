#!/usr/bin/env python3
"""
Reads benchmark_results.csv and generates line plots.

Algorithms compared:
  brute  — O(n²) nested loop
  hash   — O(n)  hash map lookup

Cases:
  no_solution      — worst case for brute (exhausts all pairs)
  solution_exists  — best/avg case for brute (may exit early)
"""

import matplotlib
matplotlib.use('Agg')

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import sys

os.makedirs('graphs', exist_ok=True)

csv_path = os.path.join(os.path.dirname(__file__), 'benchmark_results.csv')

if not os.path.exists(csv_path):
    print(f"\nERROR: {csv_path} not found!")
    print("\nPlease run the benchmark first:")
    print("  ./build/release/benchmark")
    print("\nThis will generate benchmark_results.csv with timing data.")
    sys.exit(1)


print(f"Reading CSV from: {csv_path}")
df = pd.read_csv(csv_path)
print(f"Loaded {len(df)} rows")
print(f"Algorithms : {df['algorithm'].unique().tolist()}")
print(f"Cases      : {df['case'].unique().tolist()}")
print(f"n values   : {sorted(df['n'].unique().tolist())}")

# ── Dark theme constants ──────────────────────────────────────────────────────
DARK_BG     = '#0f0f0f'
PANEL_BG    = '#1a1a1a'
GRID_COLOR  = '#2a2a2a'
SPINE_COLOR = '#333333'
TICK_COLOR  = '#888888'
LABEL_COLOR = '#cccccc'
TITLE_COLOR = '#ffffff'
LEGEND_BG   = '#1e1e1e'

# ── Colors ────────────────────────────────────────────────────────────────────
# One color per algorithm, one style per case
ALGO_COLORS = {
    'brute': '#e74c3c',  # red
    'hash':  '#2ecc71',  # green
}

CASE_STYLES = {
    'no_solution':     ('o', '-'),
    'solution_exists': ('s', '--'),
}

CASE_LABELS = {
    'no_solution':     'No solution (worst case)',
    'solution_exists': 'Solution exists (best/avg case)',
}

ALGO_LABELS = {
    'brute': 'Brute Force  O(n²)',
    'hash':  'Hash Map  O(n)',
}

# ── Style helpers ─────────────────────────────────────────────────────────────

def apply_dark_style(fig, axes):
    """Apply consistent dark theme to a figure and list of axes."""
    fig.patch.set_facecolor(DARK_BG)
    for ax in axes:
        ax.set_facecolor(PANEL_BG)
        ax.grid(True, color=GRID_COLOR, linewidth=0.8, zorder=0)
        ax.tick_params(colors=TICK_COLOR, length=0)
        ax.xaxis.label.set_color(LABEL_COLOR)
        ax.yaxis.label.set_color(LABEL_COLOR)
        ax.title.set_color(TITLE_COLOR)
        for spine in ax.spines.values():
            spine.set_edgecolor(SPINE_COLOR)
        ax.legend(
            facecolor=LEGEND_BG,
            edgecolor=SPINE_COLOR,
            labelcolor=LABEL_COLOR,
            fontsize=10,
        )

def format_time_axis(ax):
    """Auto-scale time axis: µs / ms / s depending on data range."""
    def fmt(x, _):
        if x == 0:
            return '0'
        if x < 0.001:
            return f'{x*1e6:.0f}µs'
        if x < 1.0:
            return f'{x*1000:.1f}ms'
        return f'{x:.2f}s'
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(fmt))

def save(fig, filename):
    plt.tight_layout()
    path = f'graphs/{filename}'
    fig.savefig(path, dpi=150, bbox_inches='tight', facecolor=DARK_BG)
    plt.close(fig)
    print(f'  ✓ graphs/{filename}')

# ── Graph 1: No-solution case (worst case) ────────────────────────────────────

def plot_no_solution():
    """Worst case comparison — brute must check every pair.

    This is the clearest view of O(n²) vs O(n): brute has no early exit
    so it always does n*(n-1)/2 iterations. Hash builds the table and
    scans once regardless of whether a solution exists.
    """
    fig, ax = plt.subplots(figsize=(9, 5.5))

    data = df[df['case'] == 'no_solution'].sort_values('n')

    for algo in ['brute', 'hash']:
        d = data[data['algorithm'] == algo]
        if d.empty:
            continue
        marker, linestyle = CASE_STYLES['no_solution']
        ax.plot(d['n'], d['time_seconds'],
                marker=marker, linestyle=linestyle,
                color=ALGO_COLORS[algo], label=ALGO_LABELS[algo],
                linewidth=2.2, markersize=6, markeredgewidth=0, zorder=3)

    format_time_axis(ax)
    ax.set_xlabel('Input size (n)', fontsize=11)
    ax.set_ylabel('Time', fontsize=11)
    ax.set_title('Two-Sum: No Solution  —  Worst Case', fontsize=13,
                 fontweight='bold', pad=14)
    apply_dark_style(fig, [ax])
    save(fig, 'twosum_no_solution.png')


# ── Graph 2: Solution-exists case ─────────────────────────────────────────────

def plot_solution_exists():
    """Best/avg case — brute can exit early on first match.

    Brute force with early exit is dramatically faster here, especially at
    small n where it may find the pair near the start of the array.
    Hash still pays the full O(n) table-build cost regardless of where the
    solution is, so brute can actually beat hash at small n in this case.
    """
    fig, ax = plt.subplots(figsize=(9, 5.5))

    data = df[df['case'] == 'solution_exists'].sort_values('n')

    for algo in ['brute', 'hash']:
        d = data[data['algorithm'] == algo]
        if d.empty:
            continue
        marker, linestyle = CASE_STYLES['solution_exists']
        ax.plot(d['n'], d['time_seconds'],
                marker=marker, linestyle=linestyle,
                color=ALGO_COLORS[algo], label=ALGO_LABELS[algo],
                linewidth=2.2, markersize=6, markeredgewidth=0, zorder=3)

    format_time_axis(ax)
    ax.set_xlabel('Input size (n)', fontsize=11)
    ax.set_ylabel('Time', fontsize=11)
    ax.set_title('Two-Sum: Solution Exists  —  Best/Avg Case', fontsize=13,
                 fontweight='bold', pad=14)
    apply_dark_style(fig, [ax])
    save(fig, 'twosum_solution_exists.png')


# ── Entry point ────────────────────────────────────────────────────────────────

def main():
    print("\nGenerating two-sum graphs...\n")

    if not os.path.exists('benchmark_results.csv'):
        print("ERROR: benchmark_results.csv not found!")
        return

    plot_no_solution()
    plot_solution_exists()

    print(f"\n✓ All graphs written to graphs/")
    print("\nGenerated files:")
    for f in sorted(os.listdir('graphs')):
        print(f"  - graphs/{f}")


if __name__ == '__main__':
    main()
