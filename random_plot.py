import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import sys

def main():
    # 1. Set up Argument Parser
    parser = argparse.ArgumentParser(description='Analyze algorithm optimization data (Temperature vs Cycles).')
    parser.add_argument('input_file', help='Path to the CSV file containing the data.')
    args = parser.parse_args()

    # 2. Load the data
    try:
        # Assumes comma-separated, no header row
        df = pd.read_csv(args.input_file, names=['temperature', 'cycles', 'success'])
        
        if df.empty:
            print("Error: The file is empty.")
            return

        # 3. Calculate Statistics
        avg_cycles = df['cycles'].mean()
        avg_temp = df['temperature'].mean()
        success_rate = df['success'].mean()
        
        print(f"\n--- Statistics for: {args.input_file} ---")
        print(f"Total Runs: {len(df)}")
        print(f"Mean Cycles: {avg_cycles:.2f}")
        print(f"Mean Temp:   {avg_temp:.4f}")
        print(f"Success Rate: {success_rate*100:.2f}%\n")

        # 4. Visualization
        sns.set_theme(style="whitegrid")
        
        # Create figure with subplots
        fig, axes = plt.subplots(1, 2, figsize=(14, 5))
        
        # Left plot: Temperature vs Cycles (hex plot)
        ax1 = axes[0]
        hexbin = ax1.hexbin(df['cycles'], df['temperature'], gridsize=30, cmap='YlGn', edgecolors='black', linewidths=0.2)
        ax1.set_xlabel("Cycles to Optimal")
        ax1.set_ylabel("Temperature at Optimal")
        ax1.set_title("Temperature vs Cycles")
        plt.colorbar(hexbin, ax=ax1, label='Count')
        
        # Right plot: Success Rate
        ax2 = axes[1]
        success_count = int(df['success'].sum())
        failure_count = len(df) - success_count
        colors = ['#4CB391', '#FF6B6B']
        bars = ax2.bar(['Success', 'Failure'], [success_count, failure_count], color=colors, edgecolor='black', linewidth=1.5)
        ax2.set_ylabel("Count")
        ax2.set_title(f"Success Rate: {success_rate*100:.2f}%")
        
        # Add count labels on bars
        for bar in bars:
            height = bar.get_height()
            ax2.text(bar.get_x() + bar.get_width()/2., height,
                    f'{int(height)}',
                    ha='center', va='bottom', fontweight='bold')
        
        fig.suptitle(f"Algorithm Performance: {args.input_file}", fontsize=14, fontweight='bold')
        plt.tight_layout()
        plt.show()

    except FileNotFoundError:
        print(f"Error: The file '{args.input_file}' was not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    main()