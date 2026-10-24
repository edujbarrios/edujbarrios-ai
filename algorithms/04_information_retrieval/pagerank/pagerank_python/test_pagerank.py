import math
import unittest
from pagerank import pagerank

class PageRankTests(unittest.TestCase):
    def test_reference_and_symmetry(self):
        p=pagerank([[0,1],[0,0]])
        self.assertAlmostEqual(p[0],1/2.85,places=11)
        self.assertAlmostEqual(p[1],1-1/2.85,places=11)
        self.assertAlmostEqual(sum(p),1)
        self.assertEqual(len(pagerank([[0,1,0],[0,0,1],[1,0,0]])),3)
        for x in pagerank([[0,1,0],[0,0,1],[1,0,0]]):
            self.assertAlmostEqual(x,1/3)
        self.assertEqual(pagerank([[0,0],[0,0]]),[.5,.5])
        self.assertAlmostEqual(pagerank([[0]])[0],1)
        self.assertAlmostEqual(sum(pagerank([[0,2],[1,0]])),1)
    def test_invalid(self):
        for graph in ([],[[0,1]],[[0,-1],[0,0]],
                      [[0,math.nan],[0,0]],[[0,1000001],[0,0]],
                      [[0,0],[True,0]]):
            with self.subTest(graph=graph),self.assertRaises(ValueError):
                pagerank(graph)

if __name__=="__main__":unittest.main()
