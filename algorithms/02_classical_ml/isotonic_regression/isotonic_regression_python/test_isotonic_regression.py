import unittest
from isotonic_regression import isotonic_regression as fit


class IsotonicRegressionTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(fit([3, 1, 2, 4]), [2, 2, 2, 4])
        self.assertEqual(fit([-2, 2, 0, 4]), [-2, 1, 1, 4])
        self.assertEqual(fit([1, 2, 2, 3]), [1, 2, 2, 3])
        self.assertEqual(fit([3, 2, 1]), [2, 2, 2])
        self.assertEqual(fit([1]), [1])
        self.assertEqual(fit([1_000_000, -1_000_000]), [0, 0])
        self.assertEqual(fit(list(range(512, 0, -1))), [256.5] * 512)
        original = [3, 1, 2]
        fit(original)
        self.assertEqual(original, [3, 1, 2])

    def test_invalid(self):
        for bad in ([], [0] * 513, [True], [float("nan")], [float("inf")],
                    [1000001], ["1"], None, [1, None]):
            with self.subTest(bad=type(bad)), self.assertRaises(ValueError):
                fit(bad)


if __name__ == "__main__":
    unittest.main()
