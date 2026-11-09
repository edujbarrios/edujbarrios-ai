import math
import unittest
from winsorized_mean import winsorized_mean as wmean


class WinsorizedMeanTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(wmean([1,2,3,4,100],0.2),3.0)
        self.assertAlmostEqual(wmean([1,2,3,100],0.25),2.5)
        self.assertAlmostEqual(wmean([1,2,3,100],0),26.5)
        self.assertAlmostEqual(wmean([100,3,2,1],0.25),2.5)
        self.assertAlmostEqual(wmean([1,5,100],0.49),5.0)
        self.assertAlmostEqual(wmean([42],0.49),42.0)
        self.assertAlmostEqual(wmean([7]*512,0.49),7.0)
        self.assertAlmostEqual(wmean([-1e308,1e308],0.0),0.0)
        self.assertAlmostEqual(wmean([0,2,100],0.3),34.0)
        self.assertAlmostEqual(wmean([1,2,3,100,1000],0.2),41.6)

    def test_rejects_invalid(self):
        for values, fraction in [
            ([],0),([1]*513,0),([1],-0.1),([1],0.5),([1],1.0),
            ([1],float("nan")),([1],math.inf),([1],None),([1],True),
            ([math.inf],0),([math.nan],0),([True],0),([1,"2"],0),
            (None,0),("123",0),([10**400],0),([1],10**400),
        ]:
            with self.subTest(values=str(values)[:40],fraction=fraction):
                with self.assertRaises(ValueError):
                    wmean(values,fraction)


if __name__ == "__main__":
    unittest.main()
