import math
import unittest
from f_beta_score import f_beta_score


class FBetaTests(unittest.TestCase):
    def test_reference_cases(self):
        self.assertAlmostEqual(f_beta_score(8, 2, 4), 8 / 11)
        self.assertAlmostEqual(f_beta_score(8, 2, 4, 2), 20 / 29)
        self.assertAlmostEqual(f_beta_score(8, 2, 4, 0.5), 10 / 13)
        self.assertEqual(f_beta_score(0, 0, 0), 0)
        self.assertEqual(f_beta_score(0, 10, 10), 0)
        self.assertEqual(f_beta_score(7, 0, 0), 1)
        self.assertAlmostEqual(f_beta_score(8, 2, 4, 1e-300), 0.8)
        self.assertAlmostEqual(f_beta_score(8, 2, 4, 1e300), 2 / 3)
        self.assertEqual(f_beta_score(2**31 - 1, 0, 0), 1)

    def test_invalid(self):
        for args in ((-1, 0, 0, 1), (0, 2**31, 0, 1),
                     (True, 0, 0, 1), (1.5, 0, 0, 1),
                     (1, 0, 0, 0), (1, 0, 0, -1),
                     (1, 0, 0, math.inf), (1, 0, 0, math.nan),
                     (1, 0, 0, True)):
            with self.subTest(args=args), self.assertRaises(ValueError):
                f_beta_score(*args)


if __name__ == "__main__":
    unittest.main()
