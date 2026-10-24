import math
import unittest
from temperature_softmax import temperature_softmax as softmax

class TemperatureSoftmaxTests(unittest.TestCase):
    def test_temperatures_and_offsets(self):
        logits=[0,math.log(3)]
        for t,p in ((1,.25),(.5,.1),(2,1/(1+math.sqrt(3)))):
            with self.subTest(t=t):
                v=softmax(logits,t)
                self.assertAlmostEqual(v[0],p)
                self.assertAlmostEqual(sum(v),1)
        self.assertEqual(softmax([10,10],1),[.5,.5])
        self.assertAlmostEqual(softmax([1000,1001],1)[0],1/(1+math.e))
        self.assertEqual(softmax([0],1),[1.0])
    def test_invalid(self):
        cases=(([],1),([0,1],0),([0,1],-1),
               ([0,1],float("nan")),([0,float("nan")],1),
               ([-1e308,1e308],1),([True,1],1),([0,1],True))
        for x,t in cases:
            with self.subTest(x=x,t=t),self.assertRaises(ValueError):
                softmax(x,t)

if __name__=="__main__":unittest.main()
