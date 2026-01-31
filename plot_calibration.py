import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import matplotlib.ticker as ticker

def plot_slice(data, x_col, output_name, xvalues):
    # Split data by group
    easy = data.loc[data['grp'].isin(['M', 'N'])]
    hard = data.loc[data['grp'].isin(['R', 'Q'])]

    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    for ax, subset, title in zip(axes, [easy, hard], ['Easy', 'Hard']):
        if subset.empty:
            ax.set_title(f'Group: {title} (No Data)')
            continue

        sns.scatterplot(data=subset, x=x_col, y='cycles_needed', 
                        hue='tag', alpha=0.6, ax=ax, edgecolor='w', s=60)
        
        y_max = subset['cycles_needed'].max()
        ax.set_ylim(0, y_max * 1.1)
        
        ax.set_xscale('log')
        ax.set_xticks(xvalues)
        ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
        ax.xaxis.set_minor_formatter(ticker.NullFormatter())
        
        ax.set_title(f'Group: {title} - Effect of {x_col}')
        ax.set_ylabel('Total Cycles')
        ax.set_xlabel(f'{x_col} (Log Scale)')
        ax.grid(True, which="both", ls="-", alpha=0.2)

    plt.tight_layout()
    plt.savefig(output_name)
    print(f'Plot saved as {output_name}')
    plt.close()

def main():
    parser = argparse.ArgumentParser(description='Plot variations for specific parameters.')
    parser.add_argument('csv_file', type=str, help='Path to the CSV file')
    args = parser.parse_args()

    try:
        df = pd.read_csv(args.csv_file)
    except FileNotFoundError:
        print(f"Error: File '{args.csv_file}' not found.")
        return

    # 1. Clean data
    cols_to_fix = ['cycles_needed', 'temp', 'min_temp', 'inner_iters', 'cooling']
    for col in cols_to_fix:
        if col in df.columns:
            df[col] = pd.to_numeric(df[col], errors='coerce')
    df.dropna(subset=['cycles_needed'], inplace=True)

    # List of "other" parameters that should stay constant for each plot
    all_params = ['temp', 'min_temp', 'inner_iters', 'cooling']

    # 2. Generate the 3 requested plots
    # Plot 1: Varying 'temp' (others fixed)
    others_temp = [p for p in all_params if p != 'temp']
    # Group by the constants and find the group with the most data points
    temp_varied = df.groupby(others_temp).filter(lambda x: x['temp'].nunique() > 1)
    plot_slice(temp_varied, 'temp', 'variation_temp.png', [0.05, 0.1, 0.2, 2, 20, 200])

    # Plot 2: Varying 'min_temp' (others fixed)
    others_min = [p for p in all_params if p != 'min_temp']
    min_temp_varied = df.groupby(others_min).filter(lambda x: x['min_temp'].nunique() > 1)
    plot_slice(min_temp_varied, 'min_temp', 'variation_min_temp.png', [0.001, 0.0333, 0.01, 0.1])

    # Calculate the combined parameter: cooling^(-inner_iters)
    # Using abs() or ensuring float consistency helps prevent errors
    df['cooling_factor'] = df['cooling'] ** (-df['inner_iters'])

    # Group by temp and min_temp to keep them constant
    others_combined = ['temp', 'min_temp']
    
    # Filter for cases where our new factor varies
    combined_varied = df.groupby(others_combined).filter(lambda x: x['cooling_factor'].nunique() > 1)

    # Plot using our helper function (modified to handle the specific column)
    plot_slice(combined_varied, 'cooling_factor', 'variation_cooling_combined.png', [0,1])

if __name__ == "__main__":
    main()