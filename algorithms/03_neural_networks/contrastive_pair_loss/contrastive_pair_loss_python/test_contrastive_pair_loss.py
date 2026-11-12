import unittest
from contrastive_pair_loss import contrastive_pair_loss as loss


class ContrastivePairLossTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(loss([0,0],[3,4],1,6),12.5)
        self.assertEqual(loss([0,0],[3,4],0,6),0.5)
        self.assertEqual(loss([0,0],[3,4],0,3),0)
        self.assertEqual(loss([1,2],[1,2],0,2),2)
        self.assertEqual(loss([1,2],[1,2],1,2),0)
        self.assertEqual(loss([0,0],[3,4],0,0),0)
        self.assertEqual(loss([0]*512,[1]*512,1,0),256)
        self.assertEqual(loss([0]*512,[1]*512,0,1),0)
        values = [1,2]
        loss(values,values,1,1)
        self.assertEqual(values,[1,2])

    def test_invalid(self):
        cases = [
            ([],[],1,1), ([0],[0,1],1,1),
            ([0]*513,[0]*513,1,1), ([0],[0],2,1),
            ([0],[0],True,1), ([0],[0],0.0,1),
            ([0],[0],1,-1), ([0],[0],1,1001),
            ([0],[0],1,float("inf")), ([0],[0],1,True),
            ([float("nan")],[0],1,1), ([0],[float("inf")],1,1),
            ([0],[1001],1,1), ([True],[0],1,1),
            (None,[0],1,1),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                loss(*args)


if __name__ == "__main__":
    unittest.main()
