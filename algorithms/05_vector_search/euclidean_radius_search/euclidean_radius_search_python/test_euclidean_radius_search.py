import math
import unittest
from euclidean_radius_search import euclidean_radius_search as search


class EuclideanRadiusTests(unittest.TestCase):
    def test_examples(self):
        q = [0,0]
        rows = [[1,0],[0,1],[3,4],[0,0],[-1,0]]
        self.assertEqual(search(q,rows,1),[3,0,1,4])
        self.assertEqual(search(q,rows,0),[3])
        self.assertEqual(search(q,rows,5),[3,0,1,4,2])
        self.assertEqual(search(q,rows,0.99),[3])
        self.assertEqual(search([0,0],[[1,0]],0.5),[])
        self.assertEqual(search([2],[[2],[2],[3]],0),[0,1])
        self.assertEqual(search([0,0],[[3,4],[-3,-4]],5),[0,1])
        self.assertEqual(search((0,0),((0,0),(1,0)),1),[0,1])
        self.assertEqual(search([3],[[-2],[3],[0]],2),[1])
        self.assertEqual(search([0],[[-1],[1],[0]],1),[2,0,1])

    def test_bounds(self):
        query = [0]*32
        rows = [[0]*32 for _ in range(128)]
        self.assertEqual(search(query,rows,0),list(range(128)))
        self.assertEqual(search([1000]*32,[[-1000]*32],10000),[0])
        self.assertEqual(search([0],[ [1000] ],999.9),[])

    def test_invalid(self):
        cases = [
            ([],[[0]],1),([0],[],1),([0]*33,[[0]*33],1),
            ([0],[[0]]*129,1),([0],[[0,1]],1),
            ([math.nan],[[0]],1),([0],[[math.inf]],1),
            ([1001],[[0]],1),([0],[[-1001]],1),
            ([True],[[0]],1),([0],[[True]],1),([0],[[0.0]],True),
            ([0],[[0]],-1),([0],[[0]],10001),([0],[[0]],math.nan),
            ([0],[[0]],math.inf),([0],[[0]],None),
            (None,[[0]],1),([0],None,1),([0],[[1],[]],1),
            ([0],[["a"]],1),
        ]
        for q, rows, r in cases:
            with self.subTest(case=str((q,rows,r))[:70]), self.assertRaises(ValueError):
                search(q,rows,r)


if __name__ == "__main__":
    unittest.main()
