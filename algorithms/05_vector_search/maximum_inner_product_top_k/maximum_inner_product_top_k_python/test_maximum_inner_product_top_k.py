import math
import unittest
from maximum_inner_product_top_k import maximum_inner_product_top_k as search

class MaximumInnerProductTests(unittest.TestCase):
    def test_scores_and_ties(self):
        q=[1,2]
        rows=[[1,0],[0,1],[1,2],[-1,0],[1,2]]
        self.assertEqual(search(q,rows,5),[2,4,1,0,3])
        self.assertEqual(search([0,0],rows,3),[0,1,2])
        self.assertEqual(search([1],[[5]],1),[0])
    def test_invalid(self):
        cases=(([],[[0]],1),([1],[],1),([1],[[1]],0),
               ([1],[[1]],2),([1,2],[[1]],1),
               ([1,2],[[0,0],[math.nan,1]],1),
               ([1],[[True]],1),([1e308],[[1e308]],1),
               ([1e308,1e308],[[2,2]],1),([1],[[1]],True))
        for q,rows,k in cases:
            with self.subTest(q=q,rows=rows,k=k),self.assertRaises(ValueError):
                search(q,rows,k)

if __name__=="__main__":unittest.main()
