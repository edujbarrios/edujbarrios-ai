import math
import unittest
from logsumexp import logsumexp

class LogSumExpTests(unittest.TestCase):
    def test_values_and_gradients(self):
        value,grad=logsumexp([0,0])
        self.assertAlmostEqual(value,math.log(2))
        self.assertEqual(grad,[.5,.5])
        value,grad=logsumexp([math.log(2),math.log(3)])
        self.assertAlmostEqual(value,math.log(5))
        self.assertAlmostEqual(grad[0],.4)
        self.assertAlmostEqual(grad[1],.6)
        value,grad=logsumexp([-1000,-1000])
        self.assertAlmostEqual(value,-1000+math.log(2))
        self.assertEqual(grad,[.5,.5])
        self.assertEqual(logsumexp([5]),(5,[1]))
    def test_invalid(self):
        for data in ([],[math.nan,0],[math.inf],[True],
                     [-1e308,1e308]):
            with self.subTest(data=data),self.assertRaises(ValueError):
                logsumexp(data)

if __name__=="__main__":unittest.main()
