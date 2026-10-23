import math
import unittest
from cross_entropy import cross_entropy


class CrossEntropyTests(unittest.TestCase):
    def test_uniform(self):
        self.assertAlmostEqual(cross_entropy([0, 0], 0), math.log(2))

    def test_extreme_logits(self):
        self.assertAlmostEqual(cross_entropy([1000, 0], 0), 0)
        self.assertAlmostEqual(cross_entropy([1000, 0], 1), 1000)

    def test_invalid(self):
        for logits, target in (([], 0), ([1, 2], 2), ([1, 2], -1),
                               ([1, 2], True), ([float("nan")], 0),
                               ([float("inf")], 0), (["2"], 0)):
            with self.subTest(logits=logits, target=target), self.assertRaises(ValueError):
                cross_entropy(logits, target)


if __name__ == "__main__":
    unittest.main()
