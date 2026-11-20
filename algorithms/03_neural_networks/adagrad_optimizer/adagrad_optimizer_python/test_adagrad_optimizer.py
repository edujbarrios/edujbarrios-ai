import unittest
from adagrad_optimizer import adagrad_optimizer as step


class AdaGradTests(unittest.TestCase):
    def test_step_and_state(self):
        w, h = step([1, -1], [2, -4], [0, 0], 0.1, 1)
        self.assertEqual(h, [4.0, 16.0])
        self.assertAlmostEqual(w[0], 1 - 0.2 / 3)
        self.assertAlmostEqual(w[1], -1 + 0.4 / 5)
        w2, h2 = step(w, [2, 0], h, 0.1, 1)
        self.assertEqual(h2, [8.0, 16.0])
        self.assertAlmostEqual(w2[1], w[1])
        self.assertEqual(step([], [], [], 0.1, 1), ([], []))
        self.assertEqual(step([1], [0], [0], 0.2, 1), ([1.0], [0.0]))

    def test_invalid(self):
        cases = [
            ([1], [], [0], .1, 1), ([1], [1], [-1], .1, 1),
            ([float("nan")], [1], [0], .1, 1),
            ([1], [float("inf")], [0], .1, 1),
            ([1], [1], [0], 0, 1), ([1], [1], [0], .1, 0),
            ([1], [1], [0], float("nan"), 1),
            ([1], [1e308], [0], .1, 1),
            ([1e308], [-1], [0], 1e308, 1),
            ([True], [1], [0], .1, 1),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                step(*args)


if __name__ == "__main__":
    unittest.main()
