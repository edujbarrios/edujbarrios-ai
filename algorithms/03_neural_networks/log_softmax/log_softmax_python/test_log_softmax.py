import math
import unittest
from log_softmax import log_softmax


class LogSoftmaxTests(unittest.TestCase):
    def test_reference(self):
        x=log_softmax([0,0])
        self.assertAlmostEqual(x[0],-math.log(2))
        self.assertEqual(x[0],x[1])
        y=log_softmax([0,math.log(2)])
        self.assertAlmostEqual(y[0],-math.log(3))
        self.assertAlmostEqual(y[1],math.log(2)-math.log(3))
        self.assertAlmostEqual(sum(math.exp(v) for v in y),1)
        shifted=log_softmax([100,100+math.log(2)])
        for a,b in zip(y,shifted):
            self.assertAlmostEqual(a,b)
        self.assertEqual(log_softmax([1000000,-1000000]),[0,-2000000])
        self.assertEqual(log_softmax([5]),[0])

    def test_invalid(self):
        cases=[
            [],[float('nan')],[float('inf')],[1000001],
            [True],[1j],['0'], list(range(10001))
        ]
        for case in cases:
            with self.subTest(n=len(case)):
                with self.assertRaises(ValueError):
                    log_softmax(case)


if __name__ == '__main__':
    unittest.main()
