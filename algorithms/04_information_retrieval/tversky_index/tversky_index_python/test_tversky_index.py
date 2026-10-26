import unittest
from tversky_index import tversky_index as tv


class TverskyTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(tv([1, 2], [2, 3, 4]), 0.25)
        self.assertAlmostEqual(tv([1, 2], [2, 3, 4], 0.5, 2), 2 / 11)
        self.assertAlmostEqual(tv([2, 3, 4], [1, 2], 0.5, 2), 0.25)
        self.assertEqual(tv([1, 1, 2], [1, 2]), 1)
        self.assertEqual(tv([], []), 1)
        self.assertEqual(tv([], [1]), 0)
        self.assertEqual(tv([1], []), 0)
        self.assertEqual(tv([1], [2]), 0)
        self.assertEqual(tv([-(2**31)], [-(2**31), 2**31 - 1]), 0.5)

    def test_invalid(self):
        for a, b, alpha, beta in (
            ([True], [1], 1, 1), ([2**31], [], 1, 1),
            ([1.5], [], 1, 1), (None, [], 1, 1),
            ([], [], 0, 1), ([], [], 1, -1),
            ([], [], float("nan"), 1), ([], [], 1, float("inf")),
            ([], [], True, 1), ([], [], 1, "1"),
            ([1, 2], [2, 3], 1e308, 1e308)
        ):
            with self.subTest(a=a, b=b, alpha=alpha, beta=beta), \
                    self.assertRaises(ValueError):
                tv(a, b, alpha, beta)


if __name__ == "__main__":
    unittest.main()
