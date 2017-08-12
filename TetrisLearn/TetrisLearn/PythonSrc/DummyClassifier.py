# -*- coding: utf-8 -*-
"""
Created on Sun Jul  2 15:32:04 2017

@author: Ugo
"""

import time
import numpy as np

class DummyClassifier:
    
    #Static, I guess...
    ID = 0
    
    def __init__(self):
        self.total_fit = 0
        self.total_pred = 0
        self.time = time.time()
        
        DummyClassifier.ID +=1
        self.id = DummyClassifier.ID
        
        self.log("dummy #{} created at time: {}\n".format(self.id, time.time()))
      
        
    def fit(self, X=None, Y=None):
        self.total_fit+=1
        calltime = time.time()
        
        time.sleep(2)
        
        self.log("dummy #{}, fit called at time:{}, execution time: {} Total call:{} \n".format(self.id, calltime - self.time, calltime - time.time(), self.total_fit))
        
        return self
        
    def predict(self, X=None):
        self.total_pred += 1
        
        #self.log("Total: {}: dummy Predict\n".format(self.total_pred))
        
        #if X != None:
        #    self.log(np.array2string(X))
        
        return 0.0
        
    def log(self, message, file='dummy.out'):
        out = open(file, 'a')
        out.write(message)
        out.close()
        