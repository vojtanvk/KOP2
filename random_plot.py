#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import argparse

def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description='Create summary plot from SAT problem data')
    parser.add_argument('input_file', help='Input CSV file path')
    parser.add_argument('output_file', help='Output image file path')
    args = parser.parse_args()

    # Read data from file
    data = pd.read_csv(args.input_file, header=None,
                       names=['temperature', 'cycles'])

    # Determine fully satisfied formulas
    satisfied_rows = data[data['satisfied'] == data['total']]

    # Calculate percentage of fully satisfied formulas
    fully_satisfied_percentage = 100.0 * len(satisfied_rows) / len(data)

    # Create figure: (1) satisfaction percentage, (2) flips for fully satisfied
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    # --- LEFT GRAPH ---
    # Single bar showing percentage of fully satisfied formulas
    ax1.bar(['Fully Satisfied'], [fully_satisfied_percentage], color='green', edgecolor='black')
    ax1.set_ylim(0, 100)
    ax1.set_ylabel('Percentage of Runs (%)')
    ax1.set_title('Percentage of Fully Satisfied Formulas')
    ax1.text(0, fully_satisfied_percentage + 2, f'{fully_satisfied_percentage:.1f}%', 
             ha='center', va='bottom', fontsize=12, fontweight='bold')
    ax1.grid(True, axis='y', alpha=0.3)

    # --- RIGHT GRAPH ---
    if satisfied_rows.empty:
        ax2.text(0.5, 0.5, 'No fully satisfied formulas found', ha='center', va='center')
        ax2.set_xlabel('Number of flips until satisfaction')
        ax2.set_ylabel('Count')
        ax2.set_title('Speed to Satisfaction (fully satisfied cases)')
    else:
        ax2.hist(satisfied_rows['flips'], bins=30, edgecolor='black')
        ax2.set_xlabel('Number of flips until satisfaction')
        ax2.set_ylabel('Count')
        ax2.set_title('Speed to Satisfaction (fully satisfied cases)')
        # Annotate median
        median_flips = satisfied_rows['flips'].median()
        ax2.axvline(median_flips, color='red', linestyle='--', label=f'Median = {median_flips}')
        ax2.legend()

    plt.tight_layout()
    fig.savefig(args.output_file)

if __name__ == '__main__':
    main()
