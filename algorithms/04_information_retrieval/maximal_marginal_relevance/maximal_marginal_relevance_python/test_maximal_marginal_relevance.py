import unittest
from maximal_marginal_relevance import maximal_marginal_relevance as mmr

class MMRTests(unittest.TestCase):
    def test_relevance_and_diversity(self):
        r=[.9,.8,.7]
        sim=[[1,.9,-.5],[.9,1,0],[-.5,0,1]]
        self.assertEqual(mmr(r,sim,.5,3),[0,2,1])
        self.assertEqual(mmr(r,sim,1,3),[0,1,2])
        self.assertEqual(mmr(r,sim,0,3),[0,2,1])
        self.assertEqual(mmr([1,1,1],[[1,0,0],[0,1,0],[0,0,1]],.5,3),[0,1,2])
        self.assertEqual(mmr([1],[[1]],.5,1),[0])
    def test_invalid(self):
        r=[.9,.8,.7]
        sim=[[1,.9,-.5],[.9,1,0],[-.5,0,1]]
        cases=(([],[],.5,1),(r,sim,.5,0),(r,sim,.5,4),
               (r,sim,float("nan"),1),(r,sim,1.1,1),
               (r,[[1,0],[0,1]],.5,1),
               (r,[[1,float("inf"),0],[0,1,0],[0,0,1]],.5,1),
               (r,[[1,2,0],[0,1,0],[0,0,1]],.5,1),
               ([.9,.8,-.1],sim,.5,1),(r,sim,True,1))
        for item in cases:
            with self.subTest(args=item),self.assertRaises(ValueError):
                mmr(*item)

if __name__=="__main__":
    unittest.main()
