import math
import unittest
from mean_squared_log_error import mean_squared_log_error as msle


class MSLETests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(msle([0,1,9],[0,3,9]),math.log(2)**2/3)
        self.assertEqual(msle([0,1,9],[0,1,9]),0)
        self.assertAlmostEqual(msle([0],[1]),math.log(2)**2)
        self.assertAlmostEqual(msle([0],[1000000]),math.log1p(1000000)**2)
        self.assertEqual(msle([0],[0]),0)

    def test_invalid(self):
        cases=[
            ([],[]),([0,1],[0]),([-1],[0]),([0],[-1]),
            ([1000001],[0]),([0],[1000001]),
            ([float("nan")],[1]),([0],[float("inf")]),
            ([True],[0]),(["1"],[0]),([0],[1j]),
            ([0]*10001,[0]*10001)
        ]
        for t,p in cases:
            with self.subTest(length=len(t)):
                with self.assertRaises(ValueError):
                    msle(t,p)


if __name__=="__main__":
    unittest.main()
