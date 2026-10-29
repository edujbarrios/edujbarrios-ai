import unittest
from gaussian_blur import gaussian_blur


class GaussianBlurTests(unittest.TestCase):
    def test_reference_cases(self):
        impulse = [0, 0, 0, 0, 16, 0, 0, 0, 0]
        self.assertEqual(gaussian_blur(impulse, 3, 3), [1, 2, 1, 2, 4, 2, 1, 2, 1])
        self.assertEqual(gaussian_blur([0, 0, 8], 1, 3), [0, 2, 6])
        self.assertEqual(gaussian_blur([7], 1, 1), [7])
        self.assertEqual(gaussian_blur([5] * 6, 2, 3), [5] * 6)
        self.assertEqual(gaussian_blur([0, 8, 0], 1, 3), [2, 4, 2])

    def test_invalid_inputs(self):
        good = [0, 0, 0, 0]
        cases = [
            ([], 0, 0), (good, 0, 2), (good, 2, 65),
            (good, 2, True), (good, 1.5, 2), (good[:3], 2, 2),
            ([0, 0, float("nan"), 0], 2, 2),
            ([0, 0, float("inf"), 0], 2, 2),
            ([0, 0, 1_000_001, 0], 2, 2),
            ([0, 0, True, 0], 2, 2), (None, 2, 2),
        ]
        for args in cases:
            with self.subTest(args=str(args)):
                with self.assertRaises(ValueError):
                    gaussian_blur(*args)


if __name__ == "__main__":
    unittest.main()
