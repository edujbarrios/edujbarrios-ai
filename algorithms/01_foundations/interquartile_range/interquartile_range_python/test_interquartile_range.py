import math
import unittest
from interquartile_range import interquartile_range as iqr


class InterquartileRangeTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(iqr([1,2,3,4]),1.5)
        self.assertAlmostEqual(iqr([1,2,3,4,100]),2.0)
        self.assertAlmostEqual(iqr([1,5]),2.0)
        self.assertAlmostEqual(iqr([1,2,3]),1.0)
        self.assertEqual(iqr([42]),0.0)
        self.assertEqual(iqr([7]*512),0.0)
        self.assertAlmostEqual(iqr([100,4,3,2,1]),2.0)
        self.assertAlmostEqual(iqr((-5,-3,-1,1,3)),4.0)
        self.assertAlmostEqual(iqr([1,1,1,50,50,50]),49.0)
        self.assertTrue(math.isclose(iqr([-1e308,1e308]),1e308,rel_tol=1e-12))
        self.assertEqual(iqr([0]*512),0.0)

    def test_invalid(self):
        for data in [[],[1]*513,[math.inf],[math.nan],[-math.inf],
                     [1,math.nan],[True],[1,"2"],None,"123",
                     [1+0j],[10**400],123,{"a":1}]:
            with self.subTest(data=str(data)[:40]),self.assertRaises(ValueError):
                iqr(data)
        with self.assertRaises(ValueError):
            iqr([-1e308,-1e308,1e308,1e308])


if __name__ == "__main__":
    unittest.main()
