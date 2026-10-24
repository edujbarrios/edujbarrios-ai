import math
import unittest
from markov_beam_search import markov_beam_search as beam

class BeamTests(unittest.TestCase):
    def test_best_path_and_ties(self):
        p=[.6,.4];a=[[.9,.1],[.2,.8]]
        seq,score=beam(p,a,3,2)
        self.assertEqual(seq,[0,0,0])
        self.assertAlmostEqual(score,math.log(.486))
        self.assertEqual(beam([.5,.5],[[.5,.5],[.5,.5]],3,2)[0],[0,0,0])
        self.assertEqual(beam(p,a,3,1)[0],[0,0,0])
        self.assertEqual(beam([1],[[1]],3,8),([0,0,0],0.0))
    def test_invalid(self):
        cases=(([],[],1,1),([0,1],[[.9,.1],[.2,.8]],2,2),
               ([.6,.4],[[.8,.8],[.2,.8]],2,2),
               ([.6,.4],[[.9,float("nan")],[.2,.8]],2,2),
               ([.6,.4],[[.9,.1],[.2,.8]],0,2),
               ([.6,.4],[[.9,.1],[.2,.8]],2,0),
               ([.6,.4],[[.9,.1],[.2,.8]],2,True))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):beam(*args)

if __name__=="__main__":unittest.main()
