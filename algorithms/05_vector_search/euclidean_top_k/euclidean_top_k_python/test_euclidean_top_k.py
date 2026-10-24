import unittest
from euclidean_top_k import euclidean_top_k as search

class EuclideanSearchTests(unittest.TestCase):
    def test_rank_and_ties(self):
        self.assertEqual(search([0,0],[[1,0],[0,1],[3,4],[0,0],[-1,0]],5),
                         [3,0,1,4,2])
        self.assertEqual(search([0,0],[[1,0],[1,0],[0,0]],3),[2,0,1])
        self.assertEqual(search([0,0],[[1e200,0],[2e200,0]],2),[0,1])
    def test_invalid(self):
        for q,rows,k in (([],[[0]],1),([0],[],1),
                          ([0],[[0]],0),([0],[[0]],2),
                          ([0,0],[[1,0],[0,0,0]],1),
                          ([0,0],[[0,0],[float("nan"),1]],1),
                          ([0,0],[[0,0],[True,1]],1),
                          ([1.7e308],[[-1.7e308]],1)):
            with self.subTest(q=q,rows=rows,k=k),self.assertRaises(ValueError):
                search(q,rows,k)

if __name__=="__main__":unittest.main()
