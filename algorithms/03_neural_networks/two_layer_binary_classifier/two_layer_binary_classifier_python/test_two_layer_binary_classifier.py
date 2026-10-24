import math
import unittest
from two_layer_binary_classifier import (
    two_layer_binary_classifier as train,
    two_layer_predict as predict,
)

class TwoLayerTests(unittest.TestCase):
    def test_one_epoch(self):
        w1,b1,w2,b2=train([0],[1],0.2,1)
        error=1.0/(1.0+math.exp(-0.5))-1.0
        self.assertAlmostEqual(w1,1.0)
        self.assertAlmostEqual(b1,0.5-0.2*error)
        self.assertAlmostEqual(w2,1.0-0.2*error*0.5)
        self.assertAlmostEqual(b2,-0.2*error)
    def test_training_and_inactive_relu(self):
        dead=train([-1],[1],0.2,1)
        self.assertEqual(dead[:3],(1.0,0.5,1.0))
        params=train([0,1,2,3],[0,0,1,1],0.1,500)
        self.assertLess(predict(params,0),0.1)
        self.assertGreater(predict(params,3),0.9)
    def test_invalid(self):
        cases=(([],[],0.1,1),([0],[],0.1,1),
               ([0],[2],0.1,1),([float("nan")],[1],0.1,1),
               ([True],[1],0.1,1),([0],[1],0.0,1),
               ([0],[1],0.1,0),([1e308],[0],1e308,2))
        for x,y,lr,epochs in cases:
            with self.subTest(x=x,y=y,lr=lr),self.assertRaises(ValueError):
                train(x,y,lr,epochs)
        with self.assertRaises(ValueError):
            predict([1,2,3,4],float("inf"))

if __name__=="__main__":
    unittest.main()
