import math
import unittest
from viterbi_decoding import viterbi_decoding as decode

class ViterbiTests(unittest.TestCase):
    def test_known_paths(self):
        p=[.6,.4];a=[[.7,.3],[.4,.6]];b=[[.5,.5],[.1,.9]]
        path,logp=decode([0,1,0],p,a,b)
        self.assertEqual(path,[0,0,0])
        self.assertAlmostEqual(logp,math.log(.03675))
        self.assertEqual(decode([1,1,1],p,a,b)[0],[1,1,1])
        uniform=[.5,.5];m=[[.5,.5],[.5,.5]]
        self.assertEqual(decode([0,1,0],uniform,m,m)[0],[0,0,0])
        self.assertEqual(decode([0],p,a,b)[0],[0])
    def test_invalid(self):
        p=[.6,.4];a=[[.7,.3],[.4,.6]];b=[[.5,.5],[.1,.9]]
        cases=(([],p,a,b),([0]*4097,p,a,b),([0,2],p,a,b),
               ([0,True],p,a,b),([0],[0,1],a,b),
               ([0],p,[[.7,.4],[.4,.6]],b),
               ([0],p,a,[[.5,.5],[0,1]]),
               ([0],[float("nan"),.4],a,b))
        for case in cases:
            with self.subTest(obs=case[0]),self.assertRaises(ValueError):
                decode(*case)

if __name__=="__main__":unittest.main()
