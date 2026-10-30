import math
import unittest
from soft_thresholding import soft_thresholding

class SoftThresholdingTests(unittest.TestCase):
    def test_shrinkage(self):
        self.assertEqual(soft_thresholding([-3,-0.5,0,0.5,3],1), [-2,0,0,0,2])
        self.assertEqual(soft_thresholding([1,-1],1), [0,0])
        self.assertEqual(soft_thresholding([-3,3],0), [-3,3])

    def test_extreme_singleton(self):
        self.assertEqual(soft_thresholding([1e308,-1e308],1), [1e308,-1e308])
        self.assertEqual(soft_thresholding([0],2), [0])

    def test_invalid(self):
        for values,threshold in (([],1),([math.nan],1),([math.inf],1),
                                 ([True],1),(["3"],1),([3],-1),
                                 ([3],math.inf),([3],True),([10**400],1)):
            with self.subTest(values=values,threshold=threshold), self.assertRaises(ValueError):
                soft_thresholding(values,threshold)

if __name__ == "__main__":
    unittest.main()
