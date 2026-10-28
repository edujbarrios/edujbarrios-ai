import math
import unittest
from root_mean_squared_error import root_mean_squared_error as rmse


class RMSETests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(rmse([0,1,2],[0,2,4]),math.sqrt(5/3))
        self.assertEqual(rmse([0,1,2],[0,1,2]),0)
        self.assertEqual(rmse([-1000000],[1000000]),2000000)
        self.assertEqual(rmse([0],[1]),1)
        self.assertEqual(rmse([0],[0]),0)

    def test_invalid(self):
        cases=[
            ([],[]),([0,1],[0]),([0],[float('nan')]),
            ([float('inf')],[0]),([0],[1000001]),
            ([-1000001],[0]),([True],[0]),(['1'],[0]),
            ([0],[1j]),([0]*10001,[0]*10001)
        ]
        for t,p in cases:
            with self.subTest(n=len(t)):
                with self.assertRaises(ValueError):
                    rmse(t,p)


if __name__=="__main__":
    unittest.main()
