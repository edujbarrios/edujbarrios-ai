import unittest

from mean_variance import mean_variance


class MeanVarianceTests(unittest.TestCase):
    def test_known_population_variance(self):
        self.assertEqual(mean_variance([2, 4, 4, 4, 5, 5, 7, 9]), (5.0, 4.0))

    def test_constant_and_single(self):
        self.assertEqual(mean_variance([7, 7, 7]), (7.0, 0.0))
        self.assertEqual(mean_variance([3]), (3.0, 0.0))

    def test_negative(self):
        mean, variance = mean_variance([-2, 0, 2])
        self.assertAlmostEqual(mean, 0.0)
        self.assertAlmostEqual(variance, 8 / 3)

    def test_invalid(self):
        for values in ([], [float("nan")], [float("inf")],
                       [1e308, -1e308], [True], ["3"]):
            with self.subTest(values=values), self.assertRaises(ValueError):
                mean_variance(values)


if __name__ == "__main__":
    unittest.main()
