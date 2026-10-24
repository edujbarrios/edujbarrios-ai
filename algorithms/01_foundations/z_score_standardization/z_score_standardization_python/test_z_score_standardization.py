import math
import unittest
from z_score_standardization import z_score_standardization as standardize

class ZScoreTests(unittest.TestCase):
    def test_variable_and_constant(self):
        means,stds,rows=standardize([[1,10],[2,10],[3,10]])
        self.assertEqual(means,[2.0,10.0])
        self.assertAlmostEqual(stds[0],math.sqrt(2/3))
        self.assertEqual(stds[1],0)
        self.assertAlmostEqual(rows[0][0],-math.sqrt(1.5))
        self.assertEqual(rows[1],[0,0])
        self.assertAlmostEqual(rows[2][0],math.sqrt(1.5))
        self.assertTrue(all(row[1]==0 for row in rows))
        self.assertEqual(standardize([[5,2]]),([5,2],[0,0],[[0,0]]))
    def test_invalid(self):
        for rows in ([],[[]],[[1],[1,2]],[[1,float("nan")]],
                     [[1,True]], [[-1e308],[1e308]]):
            with self.subTest(rows=rows),self.assertRaises(ValueError):
                standardize(rows)

if __name__=="__main__":unittest.main()
