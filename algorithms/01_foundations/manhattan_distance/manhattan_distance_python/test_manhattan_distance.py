import math
import unittest
from manhattan_distance import manhattan_distance


class ManhattanDistanceTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(manhattan_distance([1, 2, -1], [4, -2, 1]), 9.0)
        self.assertEqual(manhattan_distance([1, 2], [1, 2]), 0.0)
        self.assertEqual(manhattan_distance([], []), 0.0)
        self.assertEqual(manhattan_distance([-2, 3], [4, -1]), 10.0)

    def test_invalid(self):
        for a, b in [
            ([1], []), ([math.nan], [1]), ([math.inf], [1]),
            ([True], [1]), (["1"], [1]), ([1e308], [-1e308]),
            ([1e308, 1e308], [0, 0]),
        ]:
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                manhattan_distance(a, b)


if __name__ == "__main__":
    unittest.main()
