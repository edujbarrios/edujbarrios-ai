import math
import unittest
from rocchio_relevance_feedback import rocchio_relevance_feedback as rocchio

class RocchioTests(unittest.TestCase):
    def test_feedback(self):
        value=rocchio([1,1],[[2,0],[4,2]],[[0,2],[2,2]])
        self.assertAlmostEqual(value[0],3.1)
        self.assertAlmostEqual(value[1],1.45)
        self.assertEqual(rocchio([1,1],[[2,0],[4,2]],[]),[3.25,1.75])
        self.assertEqual(rocchio([0,0],[],[[1,1]]),[0,0])
        self.assertEqual(rocchio([0],[ [0] ],[]),[0])
    def test_invalid(self):
        cases=(([1],[],[]),([1],[[1,2]],[]),
               ([1],[[math.nan]],[]),([1],[[-1]],[]),
               ([1],[[1]],[[-1]]),([True],[[1]],[]),
               ([],[[1]],[]),([1],None,[]))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):rocchio(*args)

if __name__=="__main__":unittest.main()
