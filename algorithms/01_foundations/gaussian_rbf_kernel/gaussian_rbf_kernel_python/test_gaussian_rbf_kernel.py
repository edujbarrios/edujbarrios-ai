import math
import unittest
from gaussian_rbf_kernel import gaussian_rbf_kernel as rbf

class GaussianRBFTests(unittest.TestCase):
    def test_gram_matrix(self):
        k=rbf([[0],[1],[2]],1)
        self.assertEqual([k[i][i] for i in range(3)],[1,1,1])
        self.assertAlmostEqual(k[0][1],math.exp(-.5))
        self.assertAlmostEqual(k[0][2],math.exp(-2))
        self.assertEqual(k[0][1],k[1][0])
        self.assertEqual(rbf([[2],[2]],1),[[1,1],[1,1]])
        self.assertEqual(rbf([[0]],1),[[1]])
        self.assertAlmostEqual(rbf([[0],[1]],2)[0][1],math.exp(-.125))
    def test_invalid(self):
        cases=(([],1),([[]],1),([[0],[1,2]],1),
               ([[0],[1]],0),([[0],[1]],1e-7),([[0],[1]],1e7),
               ([[0],[math.nan]],1),([[0],[True]],1),
               ([[0],[1000001]],1))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):rbf(*args)

if __name__=="__main__":unittest.main()
