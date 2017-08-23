Tetris game with guidelines compliant gameplay (SRS, Ghost Piece, Hard/Soft lock) and Python scriptable AI/ML subroutines. 

The game can feature multiple boards at the same time. Number or boards, key binding for each board and more is configurable in setting.xml

When AI controlled, a board calls a python object to evaluate its move. It also regularly calls a python subroutine designed to learn from a filedump of all the move recorded. 
The learning subroutine is called on a separate thread so the game keeps playing normally during learning.

This is a working copy, game might not launch or (for instance) the settings.xml file may cause no board to be playable by a human. 

The python code provide a wrapper class for the C++/Python interface so it should be relatively easy to write custom AI class as long as they extend scikit-learn's sklearn.estimator class, or at least have fit/predict methods. 
Note that python classes must support deepCopy operation.


Tetris Guidelines : http://tetris.wikia.com/wiki/Tetris_Guideline


