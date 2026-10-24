import math
import unittest
from hits_hubs_authorities import hits_hubs_authorities as hits

class HITSTests(unittest.TestCase):
    def test_star_cycle_and_empty_graph(self):
        a,h=hits([[0,1,1],[0,0,0],[0,0,0]])
        self.assertAlmostEqual(a[0],0)
        self.assertAlmostEqual(a[1],1/math.sqrt(2))
        self.assertAlmostEqual(a[2],1/math.sqrt(2))
        self.assertEqual(h,[1,0,0])
        a,h=hits([[0,1,0],[0,0,1],[1,0,0]])
        for v in a+h:self.assertAlmostEqual(v,1/math.sqrt(3))
        self.assertEqual(hits([[0,0],[0,0]]),([0,0],[0,0]))
        self.assertEqual(hits([[1]]),([1],[1]))
    def test_invalid(self):
        for data in ([],[[0,1]],[[0,-1],[0,0]],
                     [[0,math.nan],[0,0]],[[0,2],[0,0]],
                     [[True,0],[0,1]]):
            with self.subTest(data=data),self.assertRaises(ValueError):
                hits(data)

if __name__=="__main__":unittest.main()
