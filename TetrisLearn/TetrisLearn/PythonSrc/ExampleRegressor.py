# -*- coding: utf-8 -*-
"""
Created on Mon Aug 14 13:17:29 2017

@author: Ugo
"""

import time
import ToolBox as tb

from sklearn.svm import NuSVR

from sklearn.base import BaseEstimator, RegressorMixin

class ExampleRegressor(BaseEstimator, RegressorMixin):
    
    #Static
    ID = 0
    
    def __init__(self,
                 ds = tb.DataSelector(),
                 sp = tb.ScoreParser(),
                 regressor = NuSVR(),
                 file = "./ExReg.out",
                 instanceID = 0):
        

        self.instanceID = ExampleRegressor.ID
        ExampleRegressor.ID = ExampleRegressor.ID + 1
        
        self.ds = ds
        self.sp = sp
        self.regressor = regressor
        self.file="./ExReg{}.out".format(self.ID)
        
        self.log ("Example Regressor #{}:  created at time {}\n".format(self.ID, time.time()))
        
        return
    
    def fit(self, X, Y):
        
        tFit = time.time()  
        self.regressor.fit(self.ds.transform(X), self.sp.transform(Y))       
        tFit = time.time() - tFit
        
        tScore = time.time()
        score = self.regressor.score(self.ds.transform(X), self.sp.transform(Y))
        tScore = time.time() - tScore
        
        self.log("Example Regressor #{}: fit time: {}, score time: {}, acc: {}\n".format(self.ID, tFit, tScore, score))
    
        return self
    
    
    def predict(self, X):
        
        score = self.regressor.predict(self.ds.transform(X))
        self.log("Example Regressor #{}: predict call with result: {}\n".format(self.ID, score))
        
        return score
    
    def log(self, msg):
        out = open(self.file, 'a')
        out.write(msg)
        out.close()
        
        

        
        
        
if __name__ == "__main__":
    
    import Wrapper as w
    from sklearn.model_selection import GridSearchCV
    
#    d = w.Data()
#    
#    d.loadDataFromFile("../inputData_save.raw")
#      
#    
#    ds = DataSelector()
#    lp = LabelParser()
#    
#    X = ds.transform(d.getData())
#    Y = lp.transform(d.getScores())
#    #W = d.getScores()
#    
#    clf = NuSVR()
#    
#    parameters = [
#            { 
#              'C' : np.logspace(-10,10,5),
#              'kernel' : ['rbf'],
#              'gamma'  : np.logspace(-10,10,5),
#              'nu'     : np.arange(0.001, 1, 5),
#              'shrinking' : [True, False]
#            },
#            { 
#              'C' : np.logspace(-10,10,5),
#              'kernel' : ['poly'],
#              'gamma'  : np.logspace(-10,10,5),
#              'nu'     : np.arange(0.001, 1, 5),
#              'shrinking' : [True, False],
#              'degree' : np.arange(1,5),
#              'coef0'  : np.arange(0,100, 5)
#            },
#            { 
#              'C' : np.logspace(-10,10,5),
#              'kernel' : ['sigmoid'],
#              'gamma'  : np.logspace(-10,10,5),
#              'nu'     : np.arange(0.001, 1, 5),
#              'shrinking' : [True, False],
#              'coef0'  : np.arange(0,100, 5)
#            },
#            { 
#              'C' : np.logspace(-10,10,5),
#              'kernel' : ['linear'],
#              'nu'     : np.arange(0.001, 1, 5),
#              'shrinking' : [True, False]
#            }
#                ]
#    
#    grid = GridSearchCV(clf, parameters, n_jobs=6, cv=10, error_score=-1e100)
    
    regr = w.getWrapper("ExampleRegressor")
    
    regr.fitFromFile("../inputData_save.raw")
    regr.fitFromFile("../inputData_save.raw")
    
    regr.predictFromStr("0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,5,18,6,18,6,19,4,18,5,17,6,17,6,18,4,17,1,0,0,0,0,0,2,3")

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    