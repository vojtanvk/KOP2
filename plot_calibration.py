import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import matplotlib.ticker as ticker

def main():
    parser = argparse.ArgumentParser(description='Plot overlaid scatterplots with log X-axis.')
    parser.add_argument('csv_file', type=str, help='Path to the CSV file')
    parser.add_argument('column_name', type=str, help='Name of the column to plot (X-axis, e.g., temp)')
    parser.add_argument('--output', type=str, default='scatterplot.png', help='Output image file name')
    args = parser.parse_args()

    # Load data
    try:
        data = pd.read_csv(args.csv_file)
    except FileNotFoundError:
        print(f"Error: File '{args.csv_file}' not found.")
        return

    # Ensure numeric types and handle potential errors
    data['cycles_needed'] = pd.to_numeric(data['cycles_needed'], errors='coerce')
    data[args.column_name] = pd.to_numeric(data[args.column_name], errors='coerce')
    data.dropna(subset=[args.column_name, 'cycles_needed'], inplace=True)

    # Split data by group
    easy = data.loc[data['grp'].isin(['M', 'N'])]
    hard = data.loc[data['grp'].isin(['R', 'Q'])]

    # Plot setup
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    temp_values = [0.05, 0.1, 0.2, 2, 20, 200]
    
    for ax, subset, title in zip(axes, [easy, hard], ['Easy', 'Hard']):
        # Plot
        sns.scatterplot(data=subset, x=args.column_name, y='cycles_needed', 
                        hue='sada', alpha=0.6, ax=ax, edgecolor='w', s=60)
        
        # Log Scale Settings
        ax.set_xscale('log')
        
        # Set explicit ticks so your specific temperatures show up
        ax.set_xticks(temp_values)
        
        # Format the numbers to look like 0.05 instead of 5x10^-2
        ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
        ax.xaxis.set_minor_formatter(ticker.NullFormatter()) # Clean up minor ticks
        
        ax.set_title(f'Group: {title}')
        ax.set_ylabel('Total Cycles')
        ax.set_xlabel(f'{args.column_name} (Log Scale)')
        ax.grid(True, which="both", ls="-", alpha=0.2)

    plt.tight_layout()
    plt.savefig(args.output)
    print(f'Plot saved as {args.output}')
    plt.show()

if __name__ == "__main__":
    main()