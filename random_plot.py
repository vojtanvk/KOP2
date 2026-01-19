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
        df = pd.read_csv(args.input_file, names=['temperature', 'cycles'])
        
        if df.empty:
            print("Error: The file is empty.")
            return

        # 3. Calculate Statistics
        avg_cycles = df['cycles'].mean()
        avg_temp = df['temperature'].mean()
        
        print(f"\n--- Statistics for: {args.input_file} ---")
        print(f"Total Runs: {len(df)}")
        print(f"Mean Cycles: {avg_cycles:.2f}")
        print(f"Mean Temp:   {avg_temp:.4f}")

        # 4. Visualization
        sns.set_theme(style="whitegrid")
        
        # Creating a JointPlot to see both distributions and their relationship
        g = sns.jointplot(data=df, x="cycles", y="temperature", kind="hex", color="#4CB391")
        g.set_axis_labels("Cycles to Optimal", "Temperature at Optimal")
        
        plt.show()

    except FileNotFoundError:
        print(f"Error: The file '{args.input_file}' was not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    main()