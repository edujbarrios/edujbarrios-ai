import unittest
from exact_cosine_top_k import exact_cosine_top_k as search

class CosineTopKTests(unittest.TestCase):
    def test_ranking_and_ties(self):
        vectors=[[1,0],[0,1],[-1,0],[1,1],[1,0]]
        self.assertEqual(search([1,0],vectors,4),[0,4,3,1])
        self.assertEqual(search([1],[[-2],[3],[3]],3),[1,2,0])
        self.assertEqual(search([1],[[1]],1),[0])
    def test_invalid(self):
        cases=(([],[[1]],1),([1],[],1),
               ([1],[[1]],0),([1],[[1]],2),
               ([1,0],[[1,0],[0,0]],1),
               ([1,0],[[1,0],[float("nan"),1]],1),
               ([1.7e308,1.7e308],[[1,0]],1),
               ([1,0],[[1]],1),
               ([1,0],[[True,0]],1))
        for q,values,k in cases:
            with self.subTest(query=q,k=k),self.assertRaises(ValueError):
                search(q,values,k)

if __name__=="__main__":
    unittest.main()
