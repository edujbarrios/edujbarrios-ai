import math
import unittest
from gelu import gelu

class GeluTests(unittest.TestCase):
    def test_values_and_gradients(self):
        values,d=gelu([0,1,-1,100,-100])
        t=math.tanh(0.7978845608028654*(1+0.044715))
        gate=.5*(1+t)
        deriv=gate+.5*(1-t*t)*0.7978845608028654*(1+3*0.044715)
        self.assertEqual((values[0],d[0]),(0,.5))
        self.assertAlmostEqual(values[1],gate)
        self.assertAlmostEqual(d[1],deriv)
        self.assertAlmostEqual(values[2],gate-1)
        self.assertAlmostEqual(d[2],1-deriv)
        self.assertEqual((values[3],d[3]),(100,1))
        self.assertEqual((values[4],d[4]),(0,0))
    def test_invalid(self):
        for data in ([],[float("nan")],[float("inf")],[1e308],[True]):
            with self.subTest(data=data),self.assertRaises(ValueError):
                gelu(data)

if __name__=="__main__":unittest.main()
