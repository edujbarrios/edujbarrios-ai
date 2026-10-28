import unittest
from symmetric_mean_absolute_percentage_error import (
    symmetric_mean_absolute_percentage_error as smape
)


class SMAPETests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(smape([0,1,-1],[0,2,1]),8/9)
        self.assertEqual(smape([0,1,-1],[0,1,-1]),0)
        self.assertEqual(smape([0],[0]),0)
        self.assertEqual(smape([1],[0]),2)
        self.assertEqual(smape([1],[-1]),2)
        self.assertEqual(smape([1000000],[-1000000]),2)
        self.assertAlmostEqual(smape([1],[2]),2/3)

    def test_invalid(self):
        cases=[
            ([],[]),([0,1],[0]),([0],[float("nan")]),
            ([float("inf")],[0]),([0],[1000001]),([-1000001],[0]),
            ([True],[0]),(["1"],[0]),([1j],[0]),
            ([0]*10001,[0]*10001)
        ]
        for target,prediction in cases:
            with self.subTest(length=len(target)):
                with self.assertRaises(ValueError):
                    smape(target,prediction)


if __name__=="__main__":
    unittest.main()
