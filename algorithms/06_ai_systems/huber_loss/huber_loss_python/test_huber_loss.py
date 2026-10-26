import unittest
from huber_loss import huber_loss as huber


class HuberTests(unittest.TestCase):
    def test_reference(self):
        self.assertAlmostEqual(huber([1.5, 4], [1, 2]), 0.8125)
        self.assertAlmostEqual(huber([1.5, 4], [1, 2], 2), 1.0625)
        self.assertAlmostEqual(huber([1.5, 4], [1, 2], 0.5), 0.5)
        self.assertEqual(huber([1, 2], [1, 2]), 0)
        self.assertEqual(huber([5], [4]), 0.5)
        self.assertEqual(huber([5], [4], 2), 0.5)
        self.assertEqual(huber([-2], [2]), 3.5)
        self.assertAlmostEqual(huber([1e154], [0], 1), 1e154, delta=1e142)

    def test_invalid(self):
        for a, b, delta in (([], [], 1), ([1], [1, 2], 1),
                            ([True], [1], 1), ([float("nan")], [0], 1),
                            ([float("inf")], [0], 1), ([1], [0], 0),
                            ([1], [0], -1), ([1], [0], float("inf")),
                            ([1], [0], True), ([1], [0], "1"),
                            ([1e308], [-1e308], 1), ([1e308], [0], 2),
                            (None, [0], 1)):
            with self.subTest(a=a, b=b, delta=delta), self.assertRaises(ValueError):
                huber(a, b, delta)


if __name__ == "__main__":
    unittest.main()
