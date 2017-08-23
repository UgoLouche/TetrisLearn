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
        
        #Fit Flag because prediction before a first fit may lead to errors
        self.fitted = False
        
    def fit(self, X=None, Y=None):
        
        self.wrapped_future.fit(X, Y)
        
        self.lock.acquire()
        self.wrapped_usable = copy.deepcopy(self.wrapped_future)
        self.lock.release()
        
        if self.fitted == False:
            self.fitted = True
        
        return self
        
    def predict(self, X):
        
        #Catch cases where we predict before fit
        if self.fitted == False:
            return 0
        
        self.lock.acquire()
        ret = self.wrapped_usable.predict(X)
        self.lock.release()
        
        return ret
        
    #Called by C++
    def predictFromStr(self, str):
        d = Data()
        d.loadDataFromStr(str)
        
        return self.predict(d.getData(0, 1))
    
    def fitFromFile(self, fileName=None):    
        #only read new lines
        if fileName != self.lastFileRead:
           self.lineRead = 0
           self.lastFileRead = fileName
            
            
        self.wrapped_future.log("Reading file \n")
        
        self.lineRead = self.lineRead  + self.persData.loadDataFromFile(fileName, skiprows=self.lineRead)
        
        self.wrapped_future.log("File read \n")
        
        # If that doesn't return anything, the C++ side of the call will raise an exception
        # regarding thread state (tstate) when shutting down the interpreter. Go figure.
        # -- That issue might have been fixed. Leaving it here in case it's not.
        
        #Ignore call if no data are loaded yet.
        X = self.persData.getData()
        Y = self.persData.getScores()
        if X is None or Y is None:
            return self
        
        return self.fit(X, Y)
        

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
        data = np.genfromtxt(fileName, dtype=np.float, delimiter=",", skip_header=skiprows, invalid_raise=False) #usecols=np.arange(0,266), filling_values=0)
        
        self.log(str(data.shape) + "\n")
        
        #Catch empty data (do nothing)
        if data.shape[0] == 0:
            return 0
        
        #If reading only one line, data is presented as a column vector
        if data.ndim == 1:
            if data.shape[0] == self.innerData.shape[1]:
                data = np.reshape(data, (1, self.innerData.shape[1]))
                
            #Abort if some data are simply missing or bad formatted line.
            else:
                return 0
        
        
        if (self.innerData is None):
            self.innerData = data
            return self.innerData.shape[0]
            
        else:
            prevLength = self.innerData.shape[0]
            self.innerData = np.concatenate((self.innerData, data))
            return self.innerData.shape[0] - prevLength
              
    def getData(self, idStart=-1, idEnd=-1): 
        
        if self.innerData is None:
            return None
        
        if idStart == -1:
            idStart = 0
        if idEnd == -1:
            idEnd = self.innerData.shape[0]
        
        return self.innerData[idStart:idEnd, 0:262].copy()
        
    
    def getScores(self, idStart=-1, idEnd=-1):
        
        if self.innerData is None:
            return None
        
        if idStart == -1:
            idStart = 0
        if idEnd == -1:
            idEnd = self.innerData.shape[0]
        
        self.cleanupIDs()
        return self.innerData[idStart:idEnd, 262:].copy()
        
    def rawConvert(self, str):
        #convert raw data into an npArray
        return np.reshape(np.array(str.split(",")).astype(np.float), (1,266))
    
    #IDs are a mess and need to be cleaned up to avoid duplicate
    #Working (reasonable) hypothesis is: maintain map Table for new ID. 
    #Parent should always first.
    def cleanupIDs(self):
        mapTable = np.zeros(self.innerData.shape[0] + 1) - 1
        nextID = 1
        
        for i in range(self.innerData.shape[0]):
            #register new ID conversion info
            mapTable[int(self.innerData[i,265])] = nextID
            self.innerData[i,265] = nextID
            nextID = nextID + 1
            
            #rewrite parent information
            if self.innerData[i,264] != 0:
                self.innerData[i,264] = mapTable[int(self.innerData[i,264])]

    
    def log(self, msg):
        out = open("dataReader.out", 'a')
        out.write(msg)
        out.close()

#Load a module, instanciate a class of the same name (supposedely defined in that module)
#and return a wrapper setup with that instance.  
def getWrapper(name):
    module = __import__(name)
    class_ = getattr(module, name)
    instance = class_()
    
    return Wrapper(instance)




if __name__ == "__main__":
    
    w = getWrapper("DummyClassifier")
    
    a = w.fitFromFile("../inputData_save.raw") 
    b = w.fitFromFile("../inputData_save.raw")
    b = w.fitFromFile("../inputData_save.raw")
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    