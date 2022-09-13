import socket , sys	#import socket package and sys
import keras
import numpy as np
import tensorflow as tf
from keras.optimizers import adam_v2




import sys
import trace
import threading
import time
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
    WD_SAMPLES = 10
    WD_PRED = 1
    WINDOW = WD_SAMPLES + WD_PRED

    numRCV = 0
    port = 1234				# port for UDP (8888 udp default)
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)	#create socket (DGRAM for UDP)
    host=socket.gethostbyname('127.0.0.1' )						#get the IP server
    s.bind((host,port))												#make connection

    LR = 1e-3
    model = tf.keras.Sequential([
            # Shape [batch, time, features] => [batch, lstm_units].
            # Adding more `lstm_units` just overfits more quickly.
            tf.keras.layers.LSTM(32, return_sequences=False),
            # Shape => [batch, out_steps*features].
            tf.keras.layers.Dense(WD_PRED*1,
                                kernel_initializer=tf.initializers.zeros()),
            # Shape => [batch, out_steps, features].
            tf.keras.layers.Reshape([WD_PRED, 1])
        ])
    model.compile(loss='mse', optimizer=adam_v2.Adam(lr=LR))


    values = np.zeros((WD_SAMPLES+1))
    try:
        while True:
            print("start receiving")
            data, addr = s.recvfrom(1024)						#recieve data from client
            print("received")
            num = float(data)
            values[numRCV] = num
            print(num)
            numRCV += 1
            if numRCV > WD_SAMPLES:
                tt = values[1:(WD_SAMPLES+1)].reshape(-1,WD_SAMPLES,1)
                pred = model.predict_on_batch(tt)
                s.sendto(str(((pred[0])[0])[0]).encode("utf-8"), addr)
                model.train_on_batch(values[0:WD_SAMPLES].reshape(-1,WD_SAMPLES,1), np.array(num).reshape(-1,WD_PRED,1))
                values = np.roll(values, -1)
                numRCV -= 1
            if not data:													#not data break
                break
    except:
        print("exception")
    print("END")
    s.close()															#closing connection
