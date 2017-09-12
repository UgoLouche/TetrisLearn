# -*- coding: utf-8 -*-
"""
Created on Tue Sep 12 17:52:15 2017

@author: Ugo
"""

if __name__=="__main__":
    import numpy as np
    from sklearn.model_selection import GridSearchCV
    from sklearn.svm import SVC, NuSVC
    
    import Wrapper as w
    import ExampleMimicClassifier as emc
    import ExampleRegressor as er
    import ToolBox as tb
    
    #Data
    d = w.Data()
    filePath = "../inputData_save_ManualPlay.raw"
    
    #Estimators
    est_emc = emc.ExampleMimicClassifier(lp=tb.LabelParser(mode="multiClass"))
    est_NuSVC = NuSVC()
    
    #Parameters
    p1 = { 
              'kernel' : ['rbf'],
              'gamma'  : [1e-05],
              'nu'     : np.arange(0.001, 1, 5),
              'shrinking' : [True, False]
         }
    
    #CV
    grid = GridSearchCV(est_NuSVC, p1, n_jobs=6, cv=10, error_score=-1e100)
    
    
    #Fitting
    d.loadDataFromFile(filePath)
    [X, Y] = est_emc.transform(d.getData())
    
    grid.fit(X, Y)