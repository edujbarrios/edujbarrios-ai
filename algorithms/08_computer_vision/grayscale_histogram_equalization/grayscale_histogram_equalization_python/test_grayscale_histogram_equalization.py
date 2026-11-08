import unittest
from grayscale_histogram_equalization import grayscale_histogram_equalization as eq


class HistogramEqualizationTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(eq([10,10,20,30], 2, 2), [0,0,128,255])
        self.assertEqual(eq([20,20,20], 1, 3), [20,20,20])
        self.assertEqual(eq([0,255], 1, 2), [0,255])
        self.assertEqual(eq([1], 1, 1), [1])
        self.assertEqual(eq([0,0,0,255], 2, 2), [0,0,0,255])
        self.assertEqual(eq((0,1,2,3,4), 1, 5), [0,64,128,191,255])
        self.assertEqual(eq([5,5,5,5], 2, 2), [5,5,5,5])

    def test_maximum(self):
        image = [0] * (64 * 64 - 1) + [255]
        result = eq(image, 64, 64)
        self.assertEqual(result[:4095], [0] * 4095)
        self.assertEqual(result[-1], 255)

    def test_invalid(self):
        cases = [
            ([0], 0, 1), ([0], 1, 0), ([0], 65, 1), ([0], 1, 65),
            ([0], 1.0, 1), ([0], True, 1), ([0], 1, False),
            ([], 1, 1), ([0,1], 1, 1),
            ([-1], 1, 1), ([256], 1, 1), ([1.0], 1, 1),
            ([True], 1, 1), (None, 1, 1), ("abc", 1, 3),
        ]
        for pixels, height, width in cases:
            with self.subTest(pixels=pixels, height=height, width=width):
                with self.assertRaises(ValueError):
                    eq(pixels, height, width)


if __name__ == "__main__":
    unittest.main()
