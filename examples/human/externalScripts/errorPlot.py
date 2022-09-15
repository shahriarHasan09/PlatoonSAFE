# -----------------------------------------------------------------------------------------------------
# Calculate RMSE and accumulative sum of error for all the algorithms
# Information is extracted from csv generated using joinResultsCSV.py
# -----------------------------------------------------------------------------------------------------

import pandas as pd
import matplotlib.pyplot as plt
import math
import numpy as np

kpi_results = []  # Algorithm, Config, Controller, MSE, RMSE, R2
insert_time = [40]

# Remember to change directory
for config in [10, 20]:
    for ctr_str in ["CACC"]:
        #Location where plot of accumulative sum of errors is saved
        save_plot_acc = "SB-ML/PaperResults/errorConfig" + \
            str(config) + "_accumulative.png"

        # Location of the csv with results generated by joinResultsCSV.py
        read_results = "SB-ML/PaperResults/" + str(config) + ".csv"

        # Read information from CSV
        df = pd.read_csv(read_results)
        time = df[df.Time >= insert_time[0]].loc[:, "Time"]
        time_SVR = time
        svr = df[df.Time >= insert_time[0]].loc[:, "SVR_Error"]
        nn = df[df.Time >= insert_time[0]].loc[:, "NN_Error"]
        last = df[df.Time >= insert_time[0]].loc[:, "Last_Error"]

        # Compute and save RMSE for all algoriths
        mse = np.square(abs(svr)).mean()
        rmse = math.sqrt(mse)
        kpi_results.append(["SVR", config, ctr_str, rmse])

        mse = np.square(abs(nn)).mean()
        rmse = math.sqrt(mse)
        kpi_results.append(["NN", config, ctr_str, rmse])

        mse = np.square(abs(last)).mean()
        rmse = math.sqrt(mse)
        kpi_results.append(["Last", config, ctr_str, rmse])

        # Compute accumulative sum of errors for all algorithms
        sum_err = 0
        acc_err_svr = []
        for err in svr:
            sum_err = sum_err + abs(err)
            acc_err_svr.append(sum_err)

        sum_err = 0
        acc_err_last = []
        for err in last:
            sum_err = sum_err + abs(err)
            acc_err_last.append(sum_err)

        sum_err = 0
        acc_err_nn = []
        for err in nn:
            sum_err = sum_err + abs(err)
            acc_err_nn.append(sum_err)

        plt.plot(time_SVR, acc_err_svr, label="SVR " + str(config))
        plt.plot(time, acc_err_nn, label="NN " + str(config))
        plt.plot(time, acc_err_last, label="Last " + str(config))


plt.xlabel("Simulation time (s)")
plt.ylabel("Cumulative sum (Error in s)")
plt.grid()
plt.ylim((0, 30))
plt.xlim((40, 100))
plt.legend()
plt.savefig(save_plot_acc)
plt.clf()

# Save RMSE of all algorithms into a file
df_kpi = pd.DataFrame(kpi_results, columns=[
                      "Algorithm", "Config", "Controller", "RMSE"])
df_kpi.to_csv("SB-ML/PaperResults/resultsPaper.csv")
