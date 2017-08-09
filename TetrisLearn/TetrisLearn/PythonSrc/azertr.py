# -*- coding: utf-8 -*-

import sys
import os

def multiply(a,b):    
    sys.stdout = open('fuckyou', 'a')
    print("Will compute", a, "times", b, "--debug")
    c = 0
    for i in range(0, a):
        c = c + b
    
    sys.stdout.flush()
    return c

if __name__ == "__main__":
    if "a" in locals():   
        sys.stdout = open('fuckyou', 'a')
        
        a = "second run"
        
    else:
        a = "first run"

    print(a)          
