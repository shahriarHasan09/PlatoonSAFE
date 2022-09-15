# -----------------------------------------------------------------------------------------------------
# Thread that defines and uses NN for PlatoonSAFE
# The structure of the network can be easily changed
# -----------------------------------------------------------------------------------------------------

import socket
import sys
import numpy as np
import tensorflow as tf
from keras.optimizers import adam_v2
import sys
import threading


class thread_with_trace(threading.Thread):
    def __init__(self, *args, **keywords):
        threading.Thread.__init__(self, *args, **keywords)
        self.killed = False

    def start(self):
        self.__run_backup = self.run
        self.run = self.__run
        threading.Thread.start(self)

    def __run(self):
        sys.settrace(self.globaltrace)
        self.__run_backup()
        self.run = self.__run_backup

    def globaltrace(self, frame, event, arg):
        if event == 'call':
            return self.localtrace
        else:
            return None

    def localtrace(self, frame, event, arg):
        if self.killed:
            if event == 'line':
                raise SystemExit()
        return self.localtrace

    def kill(self):
        self.killed = True


def runNN():
    # Define and run UDP socket
    port = 1234
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    host = socket.gethostbyname('127.0.0.1')
    s.bind((host, port))

    # Define NN structure
    WD_SAMPLES = 10
    WD_PRED = 1
    WINDOW = WD_SAMPLES + WD_PRED
    LR = 1e-3
    model = tf.keras.Sequential([
        tf.keras.layers.LSTM(32, return_sequences=False),
        tf.keras.layers.Dense(WD_PRED*1,
                              kernel_initializer=tf.initializers.zeros()),
        tf.keras.layers.Reshape([WD_PRED, 1])
    ])
    model.compile(loss='mse', optimizer=adam_v2.Adam(learning_rate=LR))


    numRCV = 0
    values = np.zeros((WD_SAMPLES+1))
    # Receive value from simulator, predict next, send it to simulator and train the NN with the new sample
    try:
        while True:
            # Get new sample and parse it
            data, addr = s.recvfrom(1024)
            num = float(data)
            values[numRCV] = num

            numRCV += 1

            # If at least WD_SAMPLES are received, start using NN
            if numRCV > WD_SAMPLES:
                # Select last 10 samples
                tt = values[1:(WD_SAMPLES+1)].reshape(-1, WD_SAMPLES, 1)
                # Predict next sample
                pred = model.predict_on_batch(tt)
                # Send prediction to PlatoonSAFE
                s.sendto(str(((pred[0])[0])[0]).encode("utf-8"), addr)
                # Retrain NN with the last received value
                model.train_on_batch(
                    values[0:WD_SAMPLES].reshape(-1, WD_SAMPLES, 1), np.array(num).reshape(-1, WD_PRED, 1))
                # Remove the oldest sample
                values = np.roll(values, -1)
                numRCV -= 1
            if not data:
                break
    except Exception as e:
        print(e)
    print("END")
    s.close()
