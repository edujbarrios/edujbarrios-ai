import unittest
from sigmoid import sigmoid


class SigmoidTests(unittest.TestCase):
    def test_values(self):
        a, b, c = sigmoid([-1, 0, 1])
        self.assertAlmostEqual(a, 0.2689414213699951)
        self.assertEqual(b, 0.5)
        self.assertAlmostEqual(c, 0.7310585786300049)
        self.assertAlmostEqual(a + c, 1)

    def test_extreme(self):
        self.assertEqual(sigmoid([-1e308, 1e308]), [0.0, 1.0])
        self.assertEqual(sigmoid([]), [])

    def test_invalid(self):
        for value in (float("nan"), float("inf"), "1", True):
            with self.subTest(value=value), self.assertRaises(ValueError):
                sigmoid([1, value])


if __name__ == "__main__":
    unittest.main()
