import unittest
from integral_image import integral_image as summed


class IntegralImageTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(summed([1, 2, 3, 4], 2, 2), [1, 3, 4, 10])
        self.assertEqual(summed([0, 0, 0, 0], 2, 2), [0, 0, 0, 0])
        self.assertEqual(summed([5], 1, 1), [5])
        self.assertEqual(summed([1, 2, 3], 1, 3), [1, 3, 6])
        self.assertEqual(summed([1, 2, 3], 3, 1), [1, 3, 6])
        self.assertEqual(summed((2, 0, 1, 3), 2, 2), [2, 2, 3, 6])

    def test_limits(self):
        answer = summed([255] * (64 * 64), 64, 64)
        self.assertEqual(answer[0], 255)
        self.assertEqual(answer[63], 64 * 255)
        self.assertEqual(answer[-1], 64 * 64 * 255)

    def test_invalid(self):
        cases = [
            ([0], 0, 1), ([0], 1, 0), ([0], 65, 1),
            ([0], 1, 65), ([0], 1.0, 1), ([0], True, 1),
            ([0], 1, False), ([], 1, 1), ([0, 1], 1, 1),
            ([-1], 1, 1), ([256], 1, 1), ([1.0], 1, 1),
            ([True], 1, 1), (None, 1, 1), ("0", 1, 1),
        ]
        for pixels, height, width in cases:
            with self.subTest(pixels=pixels, height=height, width=width):
                with self.assertRaises(ValueError):
                    summed(pixels, height, width)


if __name__ == "__main__":
    unittest.main()
