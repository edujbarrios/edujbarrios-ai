import math
import unittest
from dijkstra_shortest_path import dijkstra_shortest_path as shortest

class DijkstraTests(unittest.TestCase):
    def test_paths(self):
        w=[[0,2,5,0],[0,0,1,0],[0,0,0,0],[0,0,0,0]]
        self.assertEqual(shortest(w,0),[0,2,3,-1])
        self.assertEqual(shortest(w,3),[-1,-1,-1,0])
        self.assertEqual(shortest([[0,1],[1,0]],0),[0,1])
        self.assertEqual(shortest([[0]],0),[0])
        self.assertEqual(shortest([[0,2,2],[0,0,0],[0,0,0]],0),[0,2,2])
    def test_invalid(self):
        cases=(([],0),([[0,1]],0),([[0]],1),
               ([[0,-1],[0,0]],0),([[0,math.nan],[0,0]],0),
               ([[0,1000001],[0,0]],0),([[True]],0))
        for w,start in cases:
            with self.subTest(w=w,start=start),self.assertRaises(ValueError):
                shortest(w,start)

if __name__=="__main__":unittest.main()
