import pandas as pd
import matplotlib.pyplot as plt
from io import StringIO

# ---------- STEP 1: Load and Parse CSV ----------
# Path to your uploaded CSV
file_path = 'lookback_results.csv'  # Replace with actual path if running locally

# Read raw content to identify sections
with open(file_path, 'r') as file:
    raw_data = file.readlines()

# Identify section headers
sections = [i for i, line in enumerate(raw_data) if "Convergence over" in line]
sections.append(len(raw_data))  # Append end of file to capture last section

# Parse sections
parsed_sections = []
for i in range(len(sections) - 1):
    section_name = raw_data[sections[i]].strip()
    section_data = raw_data[sections[i] + 1:sections[i + 1]]
    parsed_sections.append((section_name, pd.read_csv(StringIO(''.join(section_data)))))

# Assigning sections (assuming order: simulations, periods)
first_section_name, first_section_df = parsed_sections[0]
second_section_name, second_section_df = parsed_sections[1]

# ---------- STEP 2: Clean Column Names ----------
first_section_df.columns = first_section_df.columns.str.strip()
second_section_df.columns = second_section_df.columns.str.strip()

# ---------- STEP 3: Plotting Setup ----------
# Color schemes
call_colors = ['#FFAF00', '#F46920', '#F53255', '#F857C1']
put_colors = ['#29BDFD', '#00CBBF', '#01C159', '#9DCA1C']

# ---------- STEP 4: Plot Graphs ----------

# --- Number of Simulations ---
# Call Options
plt.figure(figsize=(12, 6))
for i, col in enumerate(["Call Naive", "Call Antithetic", "Call Strat_Sampling", "Call Control Variates"]):
    plt.plot(first_section_df["nSim"], first_section_df[col], label=col, color=call_colors[i], linestyle='-')
plt.xscale("log")
plt.xlabel("Number of Simulations (log scale)")
plt.ylabel("Value")
plt.title("Lookback Option (Call) - Convergence Over Number of Simulations")
plt.legend()
plt.grid(True)
plt.show()

# Put Options
plt.figure(figsize=(12, 6))
for i, col in enumerate(["Put Naive", "Put Antithetic", "Put Strat_Sampling", "Put Control Variates"]):
    plt.plot(first_section_df["nSim"], first_section_df[col], label=col, color=put_colors[i], linestyle='-')
plt.xscale("log")
plt.xlabel("Number of Simulations (log scale)")
plt.ylabel("Value")
plt.title("Lookback Option (Put) - Convergence Over Number of Simulations")
plt.legend()
plt.grid(True)
plt.show()

# --- Periods ---
# Call Options
plt.figure(figsize=(12, 6))
for i, col in enumerate(["Call Naive", "Call Antithetic", "Call Strat_Sampling", "Call Control Variates"]):
    plt.plot(second_section_df["periods"], second_section_df[col], label=col, color=call_colors[i], linestyle='-')
plt.xlabel("Periods")
plt.ylabel("Value")
plt.title("Lookback Option (Call) - Convergence Over Periods")
plt.legend()
plt.grid(True)
plt.show()

# Put Options
plt.figure(figsize=(12, 6))
for i, col in enumerate(["Put Naive", "Put Antithetic", "Put Strat_Sampling", "Put Control Variates"]):
    plt.plot(second_section_df["periods"], second_section_df[col], label=col, color=put_colors[i], linestyle='-')
plt.xlabel("Periods")
plt.ylabel("Value")
plt.title("Lookback Option (Put) - Convergence Over Periods")
plt.legend()
plt.grid(True)
plt.show()
