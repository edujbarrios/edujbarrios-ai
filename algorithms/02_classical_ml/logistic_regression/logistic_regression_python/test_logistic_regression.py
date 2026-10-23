import unittest
from logistic_regression import logistic_regression


class LogisticRegressionTests(unittest.TestCase):
    def test_binary_separation(self):
        weight, bias = logistic_regression([0, 1, 2, 3], [0, 0, 1, 1], 0.2, 2000)
        self.assertGreater(weight, 0)
        self.assertLess(bias, -2)
        self.assertGreater(weight * 3 + bias, 2)

    def test_one_epoch(self):
        weight, bias = logistic_regression([0], [1], 0.2, 1)
        self.assertAlmostEqual(weight, 0)
        self.assertAlmostEqual(bias, 0.1)

    def test_invalid(self):
        for x, y, lr, epochs in (([], [], 0.2, 1),
                                ([0], [], 0.2, 1),
                                ([0, 1], [0, 2], 0.2, 1),
                                ([float("nan")], [0], 0.2, 1),
                                ([True], [0], 0.2, 1),
                                ([0], [1], 0, 1),
                                ([0], [1], float("inf"), 1),
                                ([0], [1], 0.2, 0),
                                ([1e308, 0], [0, 1], 1e308, 20)):
            with self.subTest(x=x, y=y, lr=lr, epochs=epochs):
                with self.assertRaises(ValueError):
                    logistic_regression(x, y, lr, epochs)


if __name__ == "__main__":
    unittest.main()
