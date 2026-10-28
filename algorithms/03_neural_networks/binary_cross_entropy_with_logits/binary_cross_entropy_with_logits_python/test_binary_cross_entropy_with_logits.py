import math
import unittest
from binary_cross_entropy_with_logits import binary_cross_entropy_with_logits as bce


class BinaryCrossEntropyWithLogitsTests(unittest.TestCase):
    def test_reference_and_boundaries(self):
        expected = (math.log(2) + 2*math.log1p(math.exp(-2)) + 2)/3
        self.assertAlmostEqual(bce([0,2,-2], [0,1,1]), expected)
        self.assertAlmostEqual(bce([0], [0.5]), math.log(2))
        self.assertAlmostEqual(bce([1000000], [1]), 0, places=12)
        self.assertAlmostEqual(bce([-1000000], [1]), 1000000)
        self.assertAlmostEqual(bce([1000000], [0.5]), 500000)
        self.assertAlmostEqual(bce([0], [0]), math.log(2))

    def test_reject_invalid(self):
        cases = [
            ([],[]), ([1,2],[1]), ([0],[-0.1]), ([0],[1.1]),
            ([float('nan')],[1]), ([float('inf')],[1]),
            ([1000001],[1]), ([0],[float('nan')]),
            ([True],[1]), (['0'],[1]), ([0],[True]),
            ([0+1j],[1]), (list(range(10001)), [0]*10001)
        ]
        for logits, labels in cases:
            with self.subTest(n=len(logits)):
                with self.assertRaises(ValueError):
                    bce(logits, labels)


if __name__ == "__main__":
    unittest.main()
