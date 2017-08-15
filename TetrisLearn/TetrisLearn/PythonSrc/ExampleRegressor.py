# -*- coding: utf-8 -*-
"""
Created on Mon Aug 14 13:17:29 2017

@author: Ugo
"""

import numpy as np
import sklearn.preprocessing as skp
import time

from sklearn.svm import NuSVR

from sklearn.base import BaseEstimator, RegressorMixin, TransformerMixin


class DataSelector(BaseEstimator, TransformerMixin):
    
    def __init__(self,
                 curr_tetra   = True,
                 next_tetra   = True,
                 hold_type    = True,
                 board        = True,
                 input_type   = True,
                 curr_coord   = True,
                 future_coord = False,
                 phase        = True):
        
         self.curr_tetra = curr_tetra
         self.next_tetra = next_tetra
         self.hold_type = hold_type
         self.board = board
         self.input_type = input_type
         self.curr_coord = curr_coord
         self.future_coord = future_coord
         self.phase = phase
    
    
    def fit(self, X, Y):
        return self
    
    
    def fit_transform(self, X, Y):
        return self.transform(X)
    
    
    def transform(self, X):
        #Select a bunch of relevant dimensions to keep.
        #Normalize each group (and subgroup) so they have the same magnitude
        #Normalize the resulting data.
        
        selector = np.zeros((262), dtype=bool)
        selector[0:7] = self.curr_tetra
        selector[7:28] = self.curr_coord
        selector[28:35] = self.hold_type
        selector[35:235] = self.board
        selector[235:242] = self.input_type
        selector[242:250] = self.curr_coord
        selector[250:258] = self.future_coord
        selector[258:262] = self.phase 
        
        self.normalize(X)
        
        #Concatenate, normalize all and return
        ret = X[:, selector]
        skp.normalize(ret, copy=True)
        
        return ret    

    
    def normalize(self, X):        
        #curr_tetra
        skp.normalize(X[:, 0:7], copy=False)
        
        #next Tetras
        skp.normalize(X[:, 7:14], copy=False)
        skp.normalize(X[:, 14:21], copy=False)
        skp.normalize(X[:, 21:28], copy=False)
        
        skp.normalize(X[:, 7:28], copy=False)
        
        #Hold type
        skp.normalize(X[:, 28:35], copy=False)
        
        #Board content
        skp.normalize(X[:, 35:235], copy=False)
        
        #input
        skp.normalize(X[:, 235:242], copy=False)
        
        #curr_coord
        skp.normalize(X[:, 242:244], copy=False)
        skp.normalize(X[:, 244:246], copy=False)
        skp.normalize(X[:, 246:248], copy=False)
        skp.normalize(X[:, 248:250], copy=False)
        
        skp.normalize(X[:, 242:250], copy=False)
        
        #Phase
        skp.normalize(X[:, 258:262], copy=False)
      
        
 

       
class LabelParser(BaseEstimator, TransformerMixin):
    
    def __init__(self, value="Score", propCoeff=0.9, maxIte=-1):
        self.value = value
        self.propCoeff= propCoeff
        self.maxIte=maxIte
        
    
    def fit(self, labels, Y=None):
        return self
    
   
    def fit_transform(self, labels, Y=None):
        return self.transform(labels)
    
    
    def transform(self, labels):
        #Select the value we will use depending on value param
        selector = np.ones((4), dtype=bool)
        if self.value=="Score":
            selector[0] = False
        elif self.value=="Line":
            selector[1] = False
            
        data = np.concatenate((np.zeros((1,3)), labels[:, selector]))    
        dataParents = data[ data[:,1].astype(int), 2]
        upd = np.zeros((data.shape[0], 1))       
        #Should be faster to compute it by iteration
        if self.maxIte == -1 or self.maxIte < data.shape[0]:
            maxIte = data.shape[0]
        else:
            maxIte = self.maxIte
                 
        for i in range(maxIte):
            #As long as each parent has only one child (except for the root, 0) is should work)
            upd[ dataParents[:].astype(int), 0] = (data[:,0] + upd[:,0]) * self.propCoeff
            
        return data[1:, 0] + upd[1:,0]
    




class ExampleRegressor(BaseEstimator, RegressorMixin):
    
    #Static
    ID = 0
    
    def __init__(self,
                 ds = DataSelector(),
                 lp = LabelParser(),
                 regressor = NuSVR(),
                 file = "./ExReg.out",
                 instanceID = 0):
        
        self.instanceID = ExampleRegressor.ID
        ExampleRegressor.ID = ExampleRegressor.ID + 1
        
        self.ds = ds
        self.lp = lp
        self.regressor = regressor
        self.file="./ExReg{}.out".format(self.ID)
        
        self.log ("Example Regressor #{}:  created at time {}\n".format(self.ID, time.time()))
        
        return
    
    def fit(self, X, Y):
        
        tFit = time.time()  
        self.regressor.fit(self.ds.transform(X), self.lp.transform(Y))       
        tFit = time.time() - tFit
        
        tScore = time.time()
        score = self.regressor.score(self.ds.transform(X), self.lp.transform(Y))
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
        
        
        
if __name__ == "__main__":
    
    import Wrapper as w
    
    #d = w.Data()
    
    #d.loadDataFromFile("../inputData_save.raw")
    
    #ds = DataSelector()
    #lp = LabelParser()
    
    #X = ds.transform(d.getData())
    #Y = lp.transform(d.getScores())
    #W = d.getScores()
    
    regr = w.getWrapper("ExampleRegressor")
    
    regr.fitFromFile("../inputData_save.raw")
    
    regr.predictFromStr("0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,5,18,6,18,6,19,4,18,5,17,6,17,6,18,4,17,1,0,0,0,0,0,2,3")

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    