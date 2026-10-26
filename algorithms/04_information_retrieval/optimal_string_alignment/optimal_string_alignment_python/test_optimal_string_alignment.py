import unittest
from optimal_string_alignment import optimal_string_alignment as osa


class OptimalStringAlignmentTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(osa([1, 2], [2, 1]), 1)
        self.assertEqual(osa([1, 2, 3], [1, 3, 2]), 1)
        self.assertEqual(osa([3, 1], [1, 2, 3]), 3)
        self.assertEqual(osa([], [1, 2]), 2)
        self.assertEqual(osa([1, 2], []), 2)
        self.assertEqual(osa([], []), 0)
        self.assertEqual(osa([1, 2, 3], [1, 2, 3]), 0)
        self.assertEqual(osa([1, 2, 3], [1, 4, 3]), 1)
        self.assertEqual(osa([1, 1], [1]), 1)
        self.assertEqual(osa([-(2**31)], [2**31 - 1]), 1)

    def test_invalid(self):
        for bad in ([True], [1.0], [2**31], [-(2**31) - 1],
                    ["1"], [None], None):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                osa(bad, [])
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                osa([], bad)


if __name__ == "__main__":
    unittest.main()
