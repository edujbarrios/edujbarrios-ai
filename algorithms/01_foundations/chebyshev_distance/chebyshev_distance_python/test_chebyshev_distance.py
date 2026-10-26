import math
import unittest

from chebyshev_distance import chebyshev_distance


class ChebyshevDistanceTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(chebyshev_distance([1, 2, -1], [4, -2, 1]), 4.0)
        self.assertEqual(chebyshev_distance([4, -2, 1], [1, 2, -1]), 4.0)
        self.assertEqual(chebyshev_distance([1, 2], [1, 2]), 0.0)
        self.assertEqual(chebyshev_distance([], []), 0.0)
        self.assertEqual(chebyshev_distance([1e308], [0]), 1e308)

    def test_invalid_inputs(self):
        for a, b in [
            ([1], []), ([math.nan], [0]), ([math.inf], [0]),
            ([True], [0]), (["1"], [0]), ([1e308], [-1e308]),
        ]:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                chebyshev_distance(a, b)


if __name__ == "__main__":
    unittest.main()
