import unittest
from binary_closing import binary_closing


class ClosingTests(unittest.TestCase):
    def test_fill_hole(self):
        image = [0] * 49
        expected = [0] * 49
        for y in range(2, 5):
            for x in range(2, 5):
                image[y*7+x] = expected[y*7+x] = 1
        image[3*7+3] = 0
        self.assertEqual(binary_closing(image, 7, 7), expected)

    def test_boundaries(self):
        self.assertEqual(binary_closing([1], 1, 1), [0])
        self.assertEqual(binary_closing([0], 1, 1), [0])
        self.assertEqual(binary_closing([0]*9, 3, 3), [0]*9)

    def test_invalid(self):
        for image, w, h in (([], 1, 1), ([1], 0, 1), ([1], 1, 65),
                            ([2], 1, 1), ([True], 1, 1),
                            ([1], True, 1), ([1.0], 1, 1), ([0, 0], 1, 1)):
            with self.subTest(image=image,w=w,h=h), self.assertRaises(ValueError):
                binary_closing(image, w, h)


if __name__ == "__main__":
    unittest.main()
