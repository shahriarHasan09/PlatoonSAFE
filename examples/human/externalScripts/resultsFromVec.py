# -----------------------------------------------------------------------------------------------------
# Generate CSV from vector files and get important parameters: collision, travel_distance, travel_time
# -----------------------------------------------------------------------------------------------------

import simUtils
import glob
import pandas as pd

data = []
# Original vector files are separated into folders with the folowing structure:
#       Config_X/100s/X_Algorithm/raw-data/
# If this structure is change, adapt the script for your own algorithms, directory locations...
for algo in ["NB", "AVG", "NN", "SVR"]:
    for config in [1, 2, 5, 7]:
        for controller in ["CACC"]:
            for seed in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]:
                # Get complete name of the .vec file. It looks for a name that contains a specific seed number and Controller
                vector_file = glob.glob(
                    "SB-ML/Config_{}/100s/{}_{}/raw-data/*{}*#{}.vec".format(config, config, algo, controller, seed))
                # Get relevant parameters from .vec file
                collision, travel_distance, travel_time = simUtils.createResultCSV(
                    vector_file[0], "insertname.csv", 100, 7)
                # Save results
                data.append([algo, config, controller, seed,
                            collision, travel_distance, travel_time])

# Create a CSV file with all the results
df_res = pd.DataFrame(data, columns=[
                      "Algorithm", "Config", "Controller", "Seed", "Collision", "StopDistance", "StopTime"])
df_res.to_csv("SB-ML/100sResults.csv")
