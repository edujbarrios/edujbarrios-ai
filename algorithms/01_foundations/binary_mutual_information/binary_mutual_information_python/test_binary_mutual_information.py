import unittest
from binary_mutual_information import binary_mutual_information as mi


class MutualInformationTests(unittest.TestCase):
    def test_information(self):
        self.assertAlmostEqual(mi([0, 0, 1, 1], [0, 0, 1, 1]), 1.0)
        self.assertAlmostEqual(mi([0, 0, 1, 1], [1, 1, 0, 0]), 1.0)
        self.assertAlmostEqual(mi([0, 0, 1, 1], [0, 1, 0, 1]), 0.0)
        self.assertAlmostEqual(mi([0, 0, 0], [1, 0, 1]), 0.0)
        self.assertAlmostEqual(mi([1], [0]), 0.0)
        self.assertAlmostEqual(mi([0, 0, 1, 1], [0, 0, 0, 1]), 0.31127812445913283)

    def test_invalid(self):
        for x, y in [
            ([], []), ([0, 1], [1]), ([0, 2], [1, 0]),
            ([1, -1], [0, 0]), ([True], [1]), ([0.0], [1]),
            ([0, 1], [0, float("nan")]), ([0], [None]),
        ]:
            with self.subTest(x=x, y=y), self.assertRaises(ValueError):
                mi(x, y)


if __name__ == "__main__":
    unittest.main()
