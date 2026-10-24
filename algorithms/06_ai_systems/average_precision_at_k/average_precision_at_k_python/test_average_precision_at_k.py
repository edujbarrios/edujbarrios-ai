import unittest
from average_precision_at_k import average_precision_at_k as ap

class AveragePrecisionTests(unittest.TestCase):
    def test_cutoffs(self):
        rel=[1,0,1,0,1]
        self.assertAlmostEqual(ap(rel,2,3),.5)
        self.assertAlmostEqual(ap(rel,4,3),5/9)
        self.assertAlmostEqual(ap(rel,5,3),34/45)
        self.assertEqual(ap([0,0],2,0),0)
        self.assertEqual(ap([1,1,1],3,3),1)
        self.assertEqual(ap([0,0,1],2,1),0)
        self.assertEqual(ap([1,1,1],2,3),1)
    def test_invalid(self):
        for rel,k,total in (([],1,0),([1,0],0,1),([1,0],3,1),
                            ([1,0,1],1,1),([0,0,2],1,2),
                            ([True],1,1),([1],True,1),
                            ([1],1,True),([1],1,-1),
                            ([1],1,2**31)):
            with self.subTest(rel=rel,k=k,total=total),self.assertRaises(ValueError):
                ap(rel,k,total)

if __name__=="__main__":unittest.main()
