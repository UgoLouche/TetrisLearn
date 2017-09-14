# -*- coding: utf-8 -*-
"""
Created on Mon Aug 21 16:36:10 2017

@author: Ugo
"""

import time
import ToolBox as tb
import pickle as pi

from sklearn.base import BaseEstimator, ClassifierMixin, TransformerMixin
from sklearn.svm import SVC, NuSVC

class ExampleMimicClassifier(BaseEstimator, ClassifierMixin, ):
    
     #Static
    ID = 0
    
    def __init__(self,
                 #clf = NuSVC(),
                 clf = pi.load(open("./PythonSrc/preTrainSVM.pick", "rb")),
                 ):
        

        self.instanceID = ExampleMimicClassifier.ID
        ExampleMimicClassifier.ID = ExampleMimicClassifier.ID + 1
 
        self.ds = tb.DataSelector(input_type=False)
        self.lp = tb.LabelParser(mode="multiClass")
        self.clf = clf
        self.file="./ExClf{}.out".format(self.ID)
        
        self.log ("Example Classifier #{}:  created at time {}\n".format(self.ID, time.time()))
        
        return
    
    
    def fit(self, X, Y):

        tProcess = time.time()
        [Xtr, Ytr] = self.transform(X)
        tProcess = time.time() - tProcess
        
        tFit = time.time()  
        self.clf.fit(Xtr, Ytr)       
        tFit = time.time() - tFit
        
        tScore = time.time()
        score = self.clf.score(Xtr, Ytr)
        tScore = time.time() - tScore
        
        self.log("Example classifier #{}: Process time: {}, fit time: {}, score time: {}, acc: {}\n".format(self.ID,tProcess, tFit, tScore, score))
    
        return self
    
    def score(self, X, Y):
        tProcess = time.time()
        [Xtr, Ytr] = self.transform(X)
        tProcess = time.time() - tProcess
        
        tScore = time.time()
        score = self.clf.score(Xtr, Ytr)
        tScore = time.time() - tScore
        
        self.log("Example classifier #{}: Process time: {}, score time: {}, acc: {}\n".format(self.ID,tProcess, tScore, score))
        
        return score
    
    
    def predict(self, X):
        
        [Xpred, Yproposed] = self.transform(X)
        pred = self.clf.predict( Xpred )
                 
        #return 0 if the input part of X doesn't match the predicted class, 1 otherwise.
        if pred == Yproposed:
            ret = 1
        else:
            ret = 0
        
        self.log("Example classifier #{}: predict call with result: {}\n".format(self.ID, ret))
        
        return ret
    
    def log(self, msg):
        out = open(self.file, 'a')
        out.write(msg)
        out.close()
        
    
    def transform(self, X):
        [retX, retY] = self.lp.transform(X)
        retX = self.ds.transform(retX)
        
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
    
    clf = w.getWrapper("ExampleMimicClassifier")
    clf.fit(d.getData())
    
    score = clf.predictFromStr("0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,5,18,6,18,6,19,4,18,5,17,6,17,6,18,4,17,1,0,0,0,0,0,2,3")
    
    
    #clf = ExampleMimicClassifier()
    
    #clf.fit(d.getData(), 0)
    
    
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
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        