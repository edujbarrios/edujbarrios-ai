import unittest
from median_filter import median_filter


class MedianFilterTests(unittest.TestCase):
    def test_reference_cases(self):
        impulse = [0, 0, 0, 0, 16, 0, 0, 0, 0]
        self.assertEqual(median_filter(impulse, 3, 3), [0] * 9)
        self.assertEqual(median_filter([0, 0, 8], 1, 3), [0, 0, 8])
        self.assertEqual(median_filter([7], 1, 1), [7])
        self.assertEqual(median_filter([5] * 6, 2, 3), [5] * 6)
        self.assertEqual(median_filter([0, 0, 0, 0, 9, 9, 0, 9, 9], 3, 3), [0, 0, 0, 0, 0, 9, 0, 9, 9])

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
                    median_filter(*args)


if __name__ == "__main__":
    unittest.main()
