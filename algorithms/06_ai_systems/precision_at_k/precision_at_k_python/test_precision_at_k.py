import unittest
from precision_at_k import precision_at_k

class PrecisionTests(unittest.TestCase):
    def test_known_values(self):
        self.assertAlmostEqual(precision_at_k([1, 0, 1, 1], 3), 2 / 3)
        self.assertEqual(precision_at_k([1, 0, 1, 1], 4), 0.75)
        self.assertEqual(precision_at_k([0, 0], 2), 0.0)

    def test_invalid(self):
        cases = (([], 1), ([1], 0), ([1], 2),
                 ([1, 0, 2], 2), ([1, True], 1),
                 ([1.0, 0], 1), ([1, 0], 1.5))
        for ranking, k in cases:
            with self.subTest(ranking=ranking, k=k):
                with self.assertRaises(ValueError):
                    precision_at_k(ranking, k)

if __name__ == "__main__":
    unittest.main()
