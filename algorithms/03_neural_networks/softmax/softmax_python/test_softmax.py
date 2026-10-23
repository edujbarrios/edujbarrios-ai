import math
import unittest
from softmax import softmax


class SoftmaxTests(unittest.TestCase):
    def test_equal_logits(self):
        result = softmax([1, 1, 1])
        for item in result:
            self.assertAlmostEqual(item, 1 / 3)
        self.assertAlmostEqual(sum(result), 1)

    def test_large_logits(self):
        result = softmax([1000, 1001])
        self.assertTrue(all(math.isfinite(x) for x in result))
        self.assertGreater(result[1], result[0])
        self.assertAlmostEqual(sum(result), 1)

    def test_singleton(self):
        self.assertEqual(softmax([1e308]), [1.0])

    def test_invalid(self):
        for values in ([], [1, float("nan")], [float("inf")],
                       [True], ["1"]):
            with self.subTest(values=values), self.assertRaises(ValueError):
                softmax(values)


if __name__ == "__main__":
    unittest.main()
