import math
import unittest
from pca_2d import pca_2d

class PCA2DTests(unittest.TestCase):
    def test_axes(self):
        mean,v,p=pca_2d([[0,0],[2,0],[4,0]])
        self.assertEqual(mean,[2,0]);self.assertEqual(v,[1,0])
        self.assertEqual(p,[-2,0,2])
        mean,v,p=pca_2d([[0,0],[0,2]])
        self.assertEqual(mean,[0,1]);self.assertEqual(v,[0,1])
        self.assertEqual(p,[-1,1])
    def test_diagonal_and_isotropy(self):
        _,v,_=pca_2d([[0,0],[2,2],[4,4]])
        self.assertAlmostEqual(v[0],math.sqrt(0.5))
        self.assertAlmostEqual(v[1],math.sqrt(0.5))
        _,v,_=pca_2d([[1,0],[-1,0],[0,1],[0,-1]])
        self.assertEqual(v,[1,0])
    def test_invalid(self):
        for rows in ([], [[0,0]], [[1,1],[1,1]], [[0],[1]],
                     [[0,0],[1,float("nan")]],
                     [[-1e308,0],[1e308,0]], [[0,True],[1,2]]):
            with self.subTest(rows=rows),self.assertRaises(ValueError):
                pca_2d(rows)

if __name__=="__main__":unittest.main()
