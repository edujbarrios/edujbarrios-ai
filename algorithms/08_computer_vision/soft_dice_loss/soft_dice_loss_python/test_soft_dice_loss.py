import unittest
from soft_dice_loss import soft_dice_loss as loss


class SoftDiceLossTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(loss([0,0],[0,0]),0)
        self.assertAlmostEqual(loss([1,0],[1,0]),0)
        self.assertAlmostEqual(loss([0,1],[1,0]),2/3)
        self.assertAlmostEqual(loss([0.5,0.5],[1,0]),1/3)
        self.assertAlmostEqual(loss([1,1],[0,0]),2/3)
        self.assertAlmostEqual(loss([0,0],[1,0]),0.5)
        self.assertAlmostEqual(loss([1],[1]),0)
        self.assertAlmostEqual(loss([0]*512,[0]*512),0)
        self.assertAlmostEqual(loss([1]*512,[1]*512),0)
        x=[.5,.25]
        loss(x,[1,0])
        self.assertEqual(x,[.5,.25])

    def test_invalid(self):
        cases=[
            ([],[]), ([0],[0,1]), ([0]*513,[0]*513),
            (None,[0]), ([0],None),
            ([float("nan")],[0]), ([float("inf")],[0]),
            ([-0.1],[0]), ([1.1],[1]), ([True],[1]),
            ([0],[True]), ([0],[0.0]), ([0],[2]),
            ([0],[-1]), (["0"],[0])
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                loss(*args)


if __name__ == "__main__":
    unittest.main()
