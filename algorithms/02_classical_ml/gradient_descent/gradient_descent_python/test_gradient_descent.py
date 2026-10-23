import unittest
from gradient_descent import gradient_descent


class GradientDescentTests(unittest.TestCase):
    def test_fit_known_line(self):
        w, b = gradient_descent([0, 1, 2, 3], [1, 3, 5, 7], 0.05, 2000)
        self.assertAlmostEqual(w, 2.0, delta=1e-3)
        self.assertAlmostEqual(b, 1.0, delta=1e-3)

    def test_horizontal_line(self):
        w, b = gradient_descent([0, 1, 2, 3], [3] * 4, 0.05, 2000)
        self.assertAlmostEqual(w, 0.0, delta=1e-3)
        self.assertAlmostEqual(b, 3.0, delta=1e-3)

    def test_invalid(self):
        for x, y, lr, epochs in (([], [], 0.05, 10),
                                ([1], [], 0.05, 10),
                                ([1], [2], 0.0, 10),
                                ([1], [2], -0.1, 10),
                                ([1], [2], 0.05, 0),
                                ([float("nan")], [2], 0.05, 10),
                                ([1], [2], float("inf"), 10)):
            with self.subTest(x=x, y=y, lr=lr, epochs=epochs):
                with self.assertRaises(ValueError):
                    gradient_descent(x, y, lr, epochs)


if __name__ == "__main__":
    unittest.main()
