#!/usr/bin/env python3
"""
Graph generator 
Reads benchmark_results.csv and generates plots for report
"""
import matplotlib
matplotlib.use('Agg')

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import os
import sys

# Create output directory
os.makedirs('graphs', exist_ok=True)

# Read data
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
print(f"Unique n values: {sorted(df['n'].unique())}")

# ── Complexity classes ────────────────────────────────────────────────────────
O_N2_ALGORITHMS    = ['insertion', 'selection', 'bubble']
O_NLOGN_ALGORITHMS = ['merge', 'quick', 'heap']
O_N_ALGORITHMS     = ['counting', 'radix']
COMPARISON_BASED   = ['insertion', 'selection', 'bubble', 'merge', 'quick', 'heap']

# ── Dark theme constants ──────────────────────────────────────────────────────
DARK_BG     = '#0f0f0f'
PANEL_BG    = '#1a1a1a'
GRID_COLOR  = '#2a2a2a'
SPINE_COLOR = '#333333'
TICK_COLOR  = '#888888'
LABEL_COLOR = '#cccccc'
TITLE_COLOR = '#ffffff'
LEGEND_BG   = '#1e1e1e'

# ── Per-algorithm colors and markers ─────────────────────────────────────────
COLORS = {
    'insertion': '#e74c3c',
    'selection': '#4a90d9',
    'bubble':    '#a855f7',
    'merge':     '#2ecc71',
    'quick':     '#f39c12',
    'heap':      '#1abc9c',
    'counting':  '#e67e22',
    'radix':     '#94a3b8',
}

MARKERS = {
    'insertion': 'o',
    'selection': 's',
    'bubble':    '^',
    'merge':     'o',
    'quick':     's',
    'heap':      '^',
    'counting':  'o',
    'radix':     's',
}

# Case styling used in per-algorithm plots
CASE_COLORS = {
    'sorted':   '#4fc3f7',
    'random':   '#a5d6a7',
    'reversed': '#ef9a9a',
}
CASE_LABELS = {
    'sorted':   'Sorted',
    'random':   'Random',
    'reversed': 'Reversed',
}
CASE_STYLES = {
    'sorted':   ('o', '-'),
    'random':   ('s', '--'),
    'reversed': ('^', ':'),
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
        return f'{x:.1f}s'
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(fmt))

def save(fig, filename):
    plt.tight_layout()
    path = f'graphs/{filename}'
    fig.savefig(path, dpi=150, bbox_inches='tight', facecolor=DARK_BG)
    plt.close(fig)
    print(f'  ✓ graphs/{filename}')

# ── Comparison graphs (random case, one line per algorithm) ───────────────────

def plot_complexity_group(algo_keys, title, filename, loglog=False):
    """One line per algorithm, random input only."""
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(9, 5.5))

    for key in algo_keys:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        ax.plot(d['n'], d['time_seconds'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    if loglog:
        ax.set_xscale('log')
        ax.set_yscale('log')
        ax.set_xlabel('Input size n  [log scale]', fontsize=11)
        ax.set_ylabel('Time  [log scale]', fontsize=11)
    else:
        ax.set_xlabel('Input size (n)', fontsize=11)
        ax.set_ylabel('Time (seconds)', fontsize=11)
        format_time_axis(ax)

    ax.set_title(f'{title}  —  random input', fontsize=13, fontweight='bold', pad=14)
    apply_dark_style(fig, [ax])
    save(fig, filename)


def plot_compare_n2():
    """O(n²): insertion, selection, bubble (unoptimized)."""
    plot_complexity_group(O_N2_ALGORITHMS, 'O(n²) Algorithms', 'compare_n2.png')


def plot_compare_nlogn():
    """O(n log n): merge, quick, heap."""
    plot_complexity_group(O_NLOGN_ALGORITHMS, 'O(n log n) Algorithms', 'compare_nlogn.png')


def plot_compare_n():
    """O(n): counting, radix."""
    plot_complexity_group(O_N_ALGORITHMS, 'O(n) Algorithms', 'compare_n.png')


def plot_compare_all_comparison_based():
    """All 6 comparison-based algorithms on a log-log plot.

    Log-log is required here: bubble at n=50k is ~7s while quicksort is
    ~0.003s. A linear scale makes the fast algorithms invisible.
    Reference slope lines are anchored to the data to confirm complexity class.
    """
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(10, 6))

    for key in COMPARISON_BASED:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        ax.plot(d['n'], d['time_seconds'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    # Reference slope lines anchored to slowest algo in each class at max n
    ns = np.array([df['n'].min(), df['n'].max()], dtype=float)

    bubble_max = random_df[random_df['algorithm'] == 'bubble']['time_seconds'].iloc[-1]
    merge_max  = random_df[random_df['algorithm'] == 'merge']['time_seconds'].iloc[-1]

    ref_n2    = ns ** 2;            ref_n2    *= bubble_max / ref_n2[-1]
    ref_nlogn = ns * np.log2(ns);   ref_nlogn *= merge_max  / ref_nlogn[-1]

    ax.plot(ns, ref_n2,    '--', color='#3a3a3a', linewidth=1.2,
            label='O(n²) slope', zorder=1)
    ax.plot(ns, ref_nlogn, '--', color='#4a4a4a', linewidth=1.2,
            label='O(n log n) slope', zorder=1)

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel('Input size n  [log scale]', fontsize=11)
    ax.set_ylabel('Time  [log scale]', fontsize=11)
    ax.set_title('All Comparison-Based Algorithms  —  random input  [log-log]',
                 fontsize=13, fontweight='bold', pad=14)

    apply_dark_style(fig, [ax])
    save(fig, 'compare_all_comparison_based.png')

# ── Normalized Graphs ────────────

def plot_normalization_n2():
    
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(10, 6))
    
    for key in O_N2_ALGORITHMS:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        d['normalized'] = d['time_seconds'] / (d['n'] * d['n'])
        ax.plot(d['n'], d['normalized'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    #ax.set_xscale('log')
    #ax.set_yscale('log')
    ax.set_xlabel('Input size n', fontsize=11)
    ax.set_ylabel('Time / n^2', fontsize=11)
    ax.set_title('Normalized (n^2) Algorithms - random input',
                 fontsize=13, fontweight='bold', pad=14)

    apply_dark_style(fig, [ax])
    save(fig, 'normalized_n2_algorithms.png')

def plot_normalization_n_log_n():
    
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(10, 6))
    
    for key in O_NLOGN_ALGORITHMS:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        d['normalized'] = d['time_seconds'] / (d['n'] * np.log2(d['n']))
        ax.plot(d['n'], d['normalized'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    #ax.set_xscale('log')
    #ax.set_yscale('log')
    ax.set_xlabel('Input size n', fontsize=11)
    ax.set_ylabel('Time / (n log n)', fontsize=11)
    ax.set_title('Normalized (n log n) Algorithms - random input',
                 fontsize=13, fontweight='bold', pad=14)

    apply_dark_style(fig, [ax])
    save(fig, 'normalized_nlogn_algorithms.png')

def plot_normalization_n():
    
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(10, 6))
    
    for key in O_N_ALGORITHMS:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        d['normalized'] = d['time_seconds'] / d['n']
        ax.plot(d['n'], d['normalized'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    #ax.set_xscale('log')
    #ax.set_yscale('log')
    ax.set_xlabel('Input size n', fontsize=11)
    ax.set_ylabel('Time / (n)', fontsize=11)
    ax.set_title('Normalized (n) Algorithms - random input',
                 fontsize=13, fontweight='bold', pad=14)

    apply_dark_style(fig, [ax])
    save(fig, 'normalized_n_algorithms.png')

def plot_time_versus_comparisons():
    
    random_df = df[df['case'] == 'random'].sort_values('n')

    fig, ax = plt.subplots(figsize=(10, 6))
    
    for key in COMPARISON_BASED:
        d = random_df[random_df['algorithm'] == key]
        if d.empty:
            continue
        d['time_elapsed_per_comparison'] = d['time_seconds'] / d['comparisons']
        ax.plot(d['n'], d['time_elapsed_per_comparison'],
                marker=MARKERS[key], color=COLORS[key],
                label=key.capitalize(), linewidth=2.2,
                markersize=6, markeredgewidth=0, zorder=3)

    #ax.set_xscale('log')
    #ax.set_yscale('log')
    ax.set_xlabel('Input size n', fontsize=11)
    ax.set_ylabel('Time / #comparisons', fontsize=11)
    ax.set_title('Time Elapsed Per Comparison - random input',
                 fontsize=13, fontweight='bold', pad=14)

    apply_dark_style(fig, [ax])
    save(fig, 'comparison_versus_execution_time.png')


# ── Per-algorithm plots (all 3 cases) ─────────────────────────────────────────

def plot_per_algorithm():
    """One plot per algorithm showing sorted / random / reversed cases."""
    algo_meta = [
        ('insertion', 'Insertion Sort',  'O(n²)'),
        ('selection', 'Selection Sort',  'O(n²)'),
        ('bubble',    'Bubble Sort',     'O(n²) — unoptimized'),
        ('merge',     'Merge Sort',      'O(n log n)'),
        ('quick',     'Quick Sort',      'O(n log n)'),
        ('heap',      'Heap Sort',       'O(n log n)'),
        ('counting',  'Counting Sort',   'O(n)'),
        ('radix',     'Radix Sort',      'O(n)'),
    ]

    for key, name, complexity in algo_meta:
        fig, ax = plt.subplots(figsize=(8, 5))

        data = df[df['algorithm'] == key]

        for case in ['sorted', 'random', 'reversed']:
            cdata = data[data['case'] == case].sort_values('n')
            if cdata.empty:
                continue
            marker, linestyle = CASE_STYLES[case]
            ax.plot(cdata['n'], cdata['time_seconds'],
                    marker=marker, linestyle=linestyle,
                    color=CASE_COLORS[case], label=CASE_LABELS[case],
                    linewidth=2, markersize=5, markeredgewidth=0, alpha=0.9)

        format_time_axis(ax)
        ax.set_xlabel('Input size (n)', fontsize=11)
        ax.set_ylabel('Time', fontsize=11)
        ax.set_title(f'{name}  ·  {complexity}', fontsize=12,
                     fontweight='bold', pad=12)

        apply_dark_style(fig, [ax])
        save(fig, f'algo_{key}.png')



# ── Entry point ────────────────────────────────────────────────────────────────

def main():
    print("Generating graphs from benchmark_results.csv...\n")

    if not os.path.exists('benchmark_results.csv'):
        print("ERROR: benchmark_results.csv not found!")
        print("Run ./build/release/benchmark first to generate data.")
        return

    print("── Complexity comparison graphs ──────────────────")
    plot_compare_n2()
    plot_compare_nlogn()
    plot_compare_n()
    plot_compare_all_comparison_based()

    print("\n── Per-algorithm case breakdown ──────────────────")
    plot_per_algorithm()

    print("\n── Normalized Graphs ──────────────────")
    plot_normalization_n2()
    plot_normalization_n_log_n()
    plot_normalization_n()

    print("\n── Time Versus Comparison Graph ──────────────────")
    plot_time_versus_comparisons()

    print(f"\n✓ All graphs written to graphs/")
    print("\nGenerated files:")
    for f in sorted(os.listdir('graphs')):
        print(f"  - graphs/{f}")


if __name__ == '__main__':
    main()
