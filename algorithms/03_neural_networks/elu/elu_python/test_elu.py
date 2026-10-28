import math
import unittest
from elu import elu


class ELUTests(unittest.TestCase):
    def test_values(self):
        result=elu([-1,0,1])
        for actual,expected in zip(result,[math.expm1(-1),0,1]):
            self.assertAlmostEqual(actual,expected)
        self.assertEqual(elu([-1000000,1000000]),[-1,1000000])
        self.assertAlmostEqual(elu([-1e-9])[0],-1e-9,places=15)
        self.assertEqual(elu([1e-9]),[1e-9])
        self.assertEqual(elu([0]),[0])

    def test_invalid(self):
        cases=[[],[float("nan")],[float("inf")],
               [1000001],[-1000001],[True],["0"],[1j],[0]*10001]
        for case in cases:
            with self.subTest(n=len(case)):
                with self.assertRaises(ValueError):
                    elu(case)


if __name__=="__main__":
    unittest.main()
