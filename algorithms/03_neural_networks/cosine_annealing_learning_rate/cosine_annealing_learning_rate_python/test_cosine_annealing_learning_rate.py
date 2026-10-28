import unittest
from cosine_annealing_learning_rate import cosine_annealing_learning_rate as rate


class CosineAnnealingTests(unittest.TestCase):
    def test_schedule(self):
        self.assertAlmostEqual(rate(1,0,0,4),1)
        self.assertAlmostEqual(rate(1,0,4,4),0)
        self.assertAlmostEqual(rate(1,0,2,4),0.5)
        self.assertAlmostEqual(rate(1,0,1,4),0.8535533905932737)
        self.assertAlmostEqual(rate(1,0,3,4),0.14644660940672627)
        self.assertEqual(rate(0.5,0.5,1,3),0.5)
        self.assertEqual(rate(1000000,0,1,1),0)

    def test_invalid(self):
        import math
        cases=[
            (0,1,1,4),(1,-1,1,4),(1000001,0,1,4),
            (math.nan,0,1,4),(math.inf,0,1,4),
            (1,0,1,0),(1,0,5,4),(1,0,1,100001),
            (True,0,1,4),(1,0,True,4),
            (1,0,1,False),(1,0,1.5,4),(1,0,1,4.0),
        ]
        for args in cases:
            with self.subTest(args=args):
                with self.assertRaises(ValueError):
                    rate(*args)


if __name__=="__main__":
    unittest.main()
