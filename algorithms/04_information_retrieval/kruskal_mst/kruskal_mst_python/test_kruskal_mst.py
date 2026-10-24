import math
import unittest
from kruskal_mst import kruskal_mst as mst

class KruskalTests(unittest.TestCase):
    def test_edges_and_cost(self):
        inf=math.inf
        g=[[0,1,4,inf],[1,0,2,5],[4,2,0,3],[inf,5,3,0]]
        self.assertEqual(mst(g),([(0,1),(1,2),(2,3)],6.0))
        self.assertEqual(mst([[0,1,1],[1,0,1],[1,1,0]]),
                         ([(0,1),(0,2)],2.0))
        self.assertEqual(mst([[0,0,2],[0,0,1],[2,1,0]]),
                         ([(0,1),(1,2)],1.0))
        self.assertEqual(mst([[0]]),([],0.0))
    def test_invalid(self):
        inf=math.inf
        for g in ([],[[0,1]],[[0,inf],[inf,0]],
                  [[0,1],[2,0]],[[0,-1],[-1,0]],
                  [[0,math.nan],[math.nan,0]],[[1,1],[1,0]],
                  [[0,True],[True,0]]):
            with self.subTest(graph=g),self.assertRaises(ValueError):
                mst(g)

if __name__=="__main__": unittest.main()
