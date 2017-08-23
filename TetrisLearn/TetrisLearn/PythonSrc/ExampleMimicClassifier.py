# -*- coding: utf-8 -*-
"""
Created on Mon Aug 21 16:36:10 2017

@author: Ugo
"""

import ExampleRegressor as er #DataSelector and LabelParser are useful :)
import time
import numpy as np

from sklearn.base import BaseEstimator, ClassifierMixin
from sklearn.utils import shuffle
from sklearn.svm import SVC, NuSVC

class ExampleMimicClassifier(BaseEstimator, ClassifierMixin):
    
     #Static
    ID = 0
    
    def __init__(self,
                 ds = er.DataSelector(),
                 clf = SVC(),
                 file = "./ExClf.out",
                 instanceID = 0):
        
        self.instanceID = ExampleMimicClassifier.ID
        ExampleMimicClassifier.ID = ExampleMimicClassifier.ID + 1
        
        self.ds = ds
        self.clf = clf
        self.file="./ExClf{}.out".format(self.ID)
        
        self.log ("Example Classifier #{}:  created at time {}\n".format(self.ID, time.time()))
        
        return
    
    
    def fit(self, X, Y):

        tProcess = time.time()
        [Xtr, Ytr] = self.transform(self.ds.transform(X))
        tProcess = time.time() - tProcess
        
        tFit = time.time()  
        self.clf.fit(Xtr, Ytr)       
        tFit = time.time() - tFit
        
        tScore = time.time()
        score = self.clf.score(Xtr, Ytr)
        tScore = time.time() - tScore
        
        self.log("Example Regressor #{}: Process time: {}, fit time: {}, score time: {}, acc: {}\n".format(self.ID,tProcess, tFit, tScore, score))
    
        return self
    
    
    def predict(self, X):
        
        ret = self.clf.predict( (self.transform(self.ds.transform(X)))[0] )
        self.log("Example Regressor #{}: predict call with result: {}\n".format(self.ID, ret))
        
        return ret
    
    def log(self, msg):
        out = open(self.file, 'a')
        out.write(msg)
        out.close()
        
    
    def transform(self, X): #Feed data, return data with label 1 and every other input possible with label 0
        retX = np.zeros((X.shape[0] * 7, X.shape[1]))
        retY = np.zeros((X.shape[0] * 7)) - 1
        
        inputValues = X[:, 235:242]        
        for i in range(7):
            retX[i*X.shape[0]:(i+1)*X.shape[0], 0:235] = X[:,0:235]
            retX[i*X.shape[0]:(i+1)*X.shape[0], 235:242] = inputValues
            retX[i*X.shape[0]:(i+1)*X.shape[0], 242:] = X[:, 242:]
            
            inputValues = np.concatenate((inputValues[:, 1:], inputValues[:,0:1]), axis=1)
        
        retY[0:X.shape[0]] = 1
        
        retX, retY = shuffle(retX, retY)
        
        return [retX, retY]
            
      
#  //Current Tetra 0-6
#	//Next Tetra 7-13 / 14-20 / 21-27
#	//Hold type 28-34
#	// Board content 35-234
#	//Input 235-241
#	// Current Tetra block coordinate 242-243 / 244-245 / 246-247 / 248-249
#	// Tetra block coordinate after input 250-251 / 252-253 / 254-255 / 256-257
#	// Tetramino phase (before move) 258-261
#	// Line cleared 262
#	// Normalized score 263
#	// previous move's ID 264
#	// move's ID 265
        
if __name__ == '__main__':
    
        
    import Wrapper as w
    
    d = w.Data()
    d.loadDataFromFile("../inputData_save.raw")
    
    
    clf = ExampleMimicClassifier()
    
    clf.fit(d.getData(), 0)
    
    
#    
#  
#    import Wrapper as w
#    from sklearn.model_selection import GridSearchCV
#    from sklearn.svm import NuSVC
#    
#    d = w.Data()
#    
#    d.loadDataFromFile("../inputData_save.raw")
#    
#    emc = ExampleMimicClassifier()
#    
#    [Xtr, Ytr] = emc.transform(d.getData())
#    
#    clf = NuSVC()
#    
#    parameters = [
#            { 
#              'kernel' : ['rbf'],
#              'gamma'  : np.logspace(-10,10,1),
#              'nu'     : np.arange(0.001, 1, 1),
#              'shrinking' : [True, False]
#            },
#            { 
#              'kernel' : ['poly'],
#              'gamma'  : np.logspace(-10,10,1),
#              'nu'     : np.arange(0.001, 1, 1),
#              'shrinking' : [True, False],
#              'degree' : np.arange(1,5,1),
#              'coef0'  : np.arange(0,100, 1)
#            },
#            { 
#              'kernel' : ['sigmoid'],
#              'gamma'  : np.logspace(-10,10,1),
#              'nu'     : np.arange(0.001, 1, 1),
#              'shrinking' : [True, False],
#              'coef0'  : np.arange(0,100, 1)
#            },
#            { 
#              'kernel' : ['linear'],
#              'nu'     : np.arange(0.001, 1, 1),
#              'shrinking' : [True, False]
#            }
#                ]
#    
#    grid = GridSearchCV(clf, parameters, n_jobs=6, cv=10, error_score=-1e100)
#     
#        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        