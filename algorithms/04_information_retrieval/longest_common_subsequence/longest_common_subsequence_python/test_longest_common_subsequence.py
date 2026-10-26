import unittest
from longest_common_subsequence import longest_common_subsequence as lcs


class LcsTests(unittest.TestCase):
    def test_sequences(self):
        self.assertEqual(lcs([1, 2, 3, 4], [2, 4, 3]), 2)
        self.assertEqual(lcs([2, 4, 3], [1, 2, 3, 4]), 2)
        self.assertEqual(lcs([1, 1, 2], [1, 2, 1]), 2)
        self.assertEqual(lcs([], [1, 2]), 0)
        self.assertEqual(lcs([1, 2], []), 0)
        self.assertEqual(lcs([], []), 0)
        self.assertEqual(lcs([1, 2], [1, 2]), 2)
        self.assertEqual(lcs([7, 7, 7], [7, 7]), 2)
        self.assertEqual(lcs([-(2**31)], [-(2**31)]), 1)
        self.assertEqual(lcs([2**31 - 1], [-(2**31)]), 0)

    def test_invalid(self):
        for bad in ([True], [1.0], [2**31], [-(2**31) - 1],
                    [None], ["bad"], None):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                lcs(bad, [1])
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                lcs([1], bad)


if __name__ == "__main__":
    unittest.main()
