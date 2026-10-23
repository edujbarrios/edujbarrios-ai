import unittest
from min_max_scaling import min_max_scaling as scale


class MinMaxScalingTests(unittest.TestCase):
    def test_values(self):
        self.assertEqual(scale([-5, 0, 5]), [0.0, 0.5, 1.0])
        self.assertEqual(scale([10, 20, 30]), [0.0, 0.5, 1.0])
        self.assertEqual(scale([2.0, 1.0]), [1.0, 0.0])

    def test_invalid(self):
        for values in ([], [1], [7, 7, 7], [1, float("nan")],
                       [1, float("inf")], [-1e308, 1e308],
                       [True, 1], ["1", "2"]):
            with self.subTest(values=values), self.assertRaises(ValueError):
                scale(values)


if __name__ == "__main__":
    unittest.main()
