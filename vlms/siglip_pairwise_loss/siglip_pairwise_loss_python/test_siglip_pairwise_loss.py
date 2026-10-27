import math
import unittest
from siglip_pairwise_loss import siglip_pairwise_loss as loss


class SiglipTests(unittest.TestCase):
    def test_reference(self):
        identity=[1,0,0,1]
        swapped=[0,1,1,0]
        expected=(math.log1p(math.exp(-1))+math.log(2))/2
        self.assertAlmostEqual(loss(identity,identity,2,2,1,0),expected)
        self.assertAlmostEqual(loss([3,0,0,2],identity,2,2,1,0),expected)
        self.assertGreater(loss(identity,swapped,2,2,1,0),expected)
        self.assertTrue(math.isfinite(loss(identity,identity,2,2,100,-10)))

    def test_invalid(self):
        a=[1,0,0,1]
        cases=[(a,a,1,2,1,0),(a,a,65,2,1,0),
               (a,a,2,0,1,0),(a,a,2,129,1,0),
               (a,a,2,2,0,0),(a,a,2,2,101,0),
               (a,a,2,2,float('nan'),0),
               (a,a,2,2,1,21),(a,a,2,2,1,float('nan')),
               (a,[0,0,0,1],2,2,1,0),
               (a,[1,0,float('inf'),1],2,2,1,0),
               (a,[1,0,1001,1],2,2,1,0),
               (a,[1,0,1],2,2,1,0),
               (a,[1,0,True,1],2,2,1,0),
               (a,a,True,2,1,0)]
        for args in cases:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    loss(*args)


if __name__ == "__main__":
    unittest.main()
