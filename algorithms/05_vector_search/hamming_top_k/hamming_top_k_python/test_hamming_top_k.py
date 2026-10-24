import unittest
from hamming_top_k import hamming_top_k

class HammingTopKTests(unittest.TestCase):
    def test_rank_and_ties(self):
        self.assertEqual(hamming_top_k(3, [1, 7, 15, 2, 3], 4), [4, 0, 1, 3])
        self.assertEqual(hamming_top_k(3, [3, 3, 0], 2), [0, 1])
        self.assertEqual(hamming_top_k(0, [0, (1 << 30) - 1], 2), [0, 1])
        self.assertEqual(hamming_top_k(3, [3], 1), [0])
    def test_invalid(self):
        cases = ((3, [], 1), (3, [1], 0), (3, [1], 2),
                 (3, [1, 2 ** 30], 1), (-1, [0], 1),
                 (2 ** 30, [0], 1), (True, [0], 1),
                 (0, [True], 1), (0, [0], True), (0, [0.5], 1))
        for q, values, k in cases:
            with self.subTest(query=q, values=values, k=k):
                with self.assertRaises(ValueError):
                    hamming_top_k(q, values, k)

if __name__ == "__main__":
    unittest.main()
