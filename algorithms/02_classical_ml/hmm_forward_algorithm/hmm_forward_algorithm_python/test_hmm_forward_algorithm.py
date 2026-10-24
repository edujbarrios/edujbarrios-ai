import math
import unittest
from hmm_forward_algorithm import hmm_forward_algorithm as forward

class HMMForwardTests(unittest.TestCase):
    def test_known_fixture_and_uniform_sequence(self):
        p=[.6,.4];a=[[.7,.3],[.4,.6]];b=[[.5,.5],[.1,.9]]
        self.assertAlmostEqual(forward([0,1,0],p,a,b),math.log(.069616))
        self.assertAlmostEqual(forward([0],p,a,b),math.log(.34))
        uniform=[.5,.5];matrix=[[.5,.5],[.5,.5]]
        self.assertAlmostEqual(forward([0,1]*50,uniform,matrix,matrix),
                               100*math.log(.5),places=10)
    def test_invalid(self):
        p=[.6,.4];a=[[.7,.3],[.4,.6]];b=[[.5,.5],[.1,.9]]
        for args in (([],p,a,b),([0]*4097,p,a,b),([0,2],p,a,b),
                     ([True],p,a,b),([0],[0,1],a,b),
                     ([0],p,[[.7,.4],[.4,.6]],b),
                     ([0],p,a,[[.5,.5],[0,1]]),
                     ([0],[float("nan"),.4],a,b)):
            with self.subTest(args=args),self.assertRaises(ValueError):
                forward(*args)

if __name__=="__main__":unittest.main()
