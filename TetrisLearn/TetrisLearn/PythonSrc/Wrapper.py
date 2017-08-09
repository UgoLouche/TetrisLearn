# -*- coding: utf-8 -*-
"""
Created on Sun Jul  2 20:14:06 2017

@author: Ugo
"""

import copy
import threading

import numpy as np

from sklearn.base import BaseEstimator, RegressorMixin

class Wrapper(BaseEstimator, RegressorMixin):
    def __init__(self, wrapped):
        self.wrapped_usable = wrapped
        self.wrapped_future = copy.deepcopy(wrapped)
        self.lock = threading.Lock()
        
        #Keep track of what line have been read already.
        self.persData = Data()
        self.lastFileRead=""
        self.LineRead = 0
        
    def fit(self, X=None, Y=None):
        self.wrapped_future.fit(X, Y)
        
        self.lock.acquire()
        self.wrapped_usable = copy.deepcopy(self.wrapped_future)
        self.lock.release()
        return self
        
    def predict(self, X):
        self.lock.acquire()
        ret = self.wrapped_usable.predict(X)
        self.lock.release()
        
        return ret
        
    #Called by C++
    def predictFromStr(self, str):
        d = Data()
        d.loadDataFromStr(str)
        
        return self.predict(d.getData(0, 1))
    
    def fitFromFile(self, fileName):    
        #only read new lines
        if fileName != self.lastFileRead:
            self.lineRead = 0
            self.lastFileRead = fileName
        
        self.lineRead = self.lineRead  + self.persData.loadDataFromFile(fileName, skiprows=self.lineRead)
        
        self.fit(self.persData.getData(), self.persData.getScores())
        

### Data Wrapper   
#Keep it simple (ie str->nparray) and leave pre-processing to the user.
#Data denotes the data themselves (duh)
#Scores denotes the cols 262-265 (ie. linesCleared, NormalizedScore, PreviousID, moveID)
class Data:
    def __init__(self):
        self.innerData = None        
                
    def loadDataFromStr(self, str):
        if (self.innerData==None):
            self.innerData = self.rawConvert(str)
        else:
            self.innerData = np.concatenate((self.innerData, self.rawConvert(str)))
    
    #Load lines from file and return the number of lines read
    def loadDataFromFile(self, fileName, skiprows=0):
        data = np.loadtxt(fileName, dtype=np.float, delimiter=",", skiprows=skiprows)
        
        if (self.innerData==None):
            self.innerData = data
            return self.innerData.shape[0]
            
        else:
            prevLength = self.innerData.shape[0]
            self.innerData = np.concatenate((self.innerData, data))
            return self.innerData.shape[0] - prevLength
              
    def getData(self, idStart=-1, idEnd=-1):    
        if idStart == -1:
            idStart = 0
        if idEnd == -1:
            idEnd = self.innerData.shape[0]
        
        return self.innerData[idStart:idEnd, 0:262]
        
    
    def getScores(self, idStart=-1, idEnd=-1):
        if idStart == -1:
            idStart = 0
        if idEnd == -1:
            idEnd = self.innerData.shape[0]
        
        return self.innerData[idStart:idEnd, 262:]
        
    def rawConvert(self, str):
        #convert raw data into an npArray
        return np.reshape(np.array(str.split(",")).astype(np.float), (1,266))

#Load a module, instanciate a class of the same name (supposedely defined in that module)
#and return a wrapper setup with that instance.  
def getWrapper(name):
    module = __import__(name)
    class_ = getattr(module, name)
    instance = class_()
    
    return Wrapper(instance)
    
    