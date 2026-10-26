import unittest
from overlap_coefficient import overlap_coefficient as overlap


class OverlapCoefficientTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(overlap([1, 1, 2], [1, 2, 3, 4]), 1)
        self.assertEqual(overlap([1, 2, 3, 4], [1, 1, 2]), 1)
        self.assertEqual(overlap([1, 2], [2, 3]), 0.5)
        self.assertEqual(overlap([-5, 3], [3, 9]), 0.5)
        self.assertEqual(overlap([], []), 1)
        self.assertEqual(overlap([], [1]), 0)
        self.assertEqual(overlap([1], []), 0)
        self.assertEqual(overlap([1], [2]), 0)
        self.assertEqual(overlap([-(2**31)], [-(2**31), 2**31 - 1]), 1)

    def test_invalid(self):
        for invalid in ([True], [1.5], [2**31], [-(2**31) - 1],
                        ["1"], [None], None):
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                overlap(invalid, [1])
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                overlap([1], invalid)


if __name__ == "__main__":
    unittest.main()
