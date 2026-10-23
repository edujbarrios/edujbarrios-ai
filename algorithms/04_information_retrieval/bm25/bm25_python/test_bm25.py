import math
import unittest
from bm25 import bm25

class BM25Tests(unittest.TestCase):
    def test_known_value(self):
        score = bm25(100, 10, 3, 100, 100.0, 1.2, 0.75)
        expected = math.log1p(90.5 / 10.5) * 3.0 * 2.2 / 4.2
        self.assertAlmostEqual(score, expected)
        self.assertEqual(bm25(100, 10, 0, 0, 100.0), 0.0)

    def test_document_length_affects_score(self):
        shorter = bm25(100, 10, 3, 50, 100)
        longer = bm25(100, 10, 3, 200, 100)
        self.assertGreater(shorter, longer)

    def test_invalid(self):
        cases = ((0, 0, 0, 0, 100, 1.2, 0.75),
                 (100, 101, 1, 10, 100, 1.2, 0.75),
                 (100, 10, 11, 10, 100, 1.2, 0.75),
                 (100, 10, 1, 10, 0, 1.2, 0.75),
                 (100, 10, 1, 10, 100, 0.0, 0.75),
                 (100, 10, 1, 10, 100, 1.2, float("nan")),
                 (100, 10, True, 10, 100, 1.2, 0.75))
        for case in cases:
            with self.subTest(case=case), self.assertRaises(ValueError):
                bm25(*case)

if __name__ == "__main__":
    unittest.main()
