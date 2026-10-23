import math
import unittest
from tf_idf import tf_idf

class TfIdfTests(unittest.TestCase):
    def test_known_value(self):
        self.assertAlmostEqual(tf_idf(3, 100, 10), 3.0 * math.log(10.0))

    def test_zero_and_common_term(self):
        self.assertEqual(tf_idf(0, 100, 10), 0.0)
        self.assertEqual(tf_idf(12, 100, 100), 0.0)

    def test_invalid(self):
        for case in ((1, 0, 0), (1, 100, 0), (1, 100, 101),
                     (-1, 100, 10), (True, 100, 10), (1.0, 100, 10)):
            with self.subTest(case=case), self.assertRaises(ValueError):
                tf_idf(*case)

if __name__ == "__main__":
    unittest.main()
