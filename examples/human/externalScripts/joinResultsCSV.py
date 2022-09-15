# -----------------------------------------------------------------------------------------------------
# Script that helps creating a CSV file that contains predictions and errors for SVR and NN
# -----------------------------------------------------------------------------------------------------

import pandas as pd
from statistics import mean


def pred_mean(last_delay):
    received_delays = []
    pred_delay = []
    for delay in last_delay:
        received_delays.append(delay)
        pred_delay.append(mean(received_delays))
    return pred_delay


"""
It is necessary to extract csv files of the predicted delay and the error of the algorithms from the vector files.
For that:
1- Open vector file in OMNet++
2- Select delay/error vector
3- Rigth click -> Export Data -> CSV for SpreadSeeths.
4- Save them with specific names and adapt the code below for that
"""
for config in [10, 20]:
    aa = pd.read_csv("SB-ML/PaperResults/NN-{}-delay.csv".format(
        config), index_col=False)
    bb = pd.read_csv("SB-ML/PaperResults/NN-{}-error.csv".format(
        config), index_col=False)
    cc = pd.read_csv("SB-ML/PaperResults/SVR-{}-delay.csv".format(
        config), index_col=False)
    dd = pd.read_csv("SB-ML/PaperResults/SVR-{}-error.csv".format(
        config), index_col=False)

    # NN starts predicting after the first 10 values so, we remove the first 10 from SVR
    svr_delay = 10

    col_time = aa.iloc[:, 0]
    col_nn_delay = aa.iloc[:, 1]
    col_nn_error = bb.iloc[:, 1]
    col_svr_delay = (cc.iloc[svr_delay:, 1])
    col_svr_delay = col_svr_delay.reset_index(drop=True)
    col_svr_error = (dd.iloc[svr_delay:, 1])
    col_svr_error = col_svr_error.reset_index(drop=True)
    col_real_delay = col_nn_delay - col_nn_error
    col_last_delay = col_real_delay.shift(1)
    col_last_delay[0] = 0
    col_last_error = col_last_delay - col_real_delay

    df = pd.DataFrame({'Time': col_time, 'NN_Delay': col_nn_delay, 'NN_Error': col_nn_error, 'SVR_Delay': col_svr_delay, 'SVR_Error': col_svr_error,
                       'Last_Delay': col_last_delay, 'Last_Error': col_last_error, 'Real_Delay': col_real_delay})
    df.to_csv("SB-ML/PaperResults/{}.csv".format(config), index=False)

print(aa)
