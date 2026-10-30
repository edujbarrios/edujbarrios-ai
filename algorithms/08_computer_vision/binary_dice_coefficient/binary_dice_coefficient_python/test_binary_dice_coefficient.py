import unittest
from binary_dice_coefficient import binary_dice_coefficient as dice

class BinaryDiceTests(unittest.TestCase):
    def test_overlaps(self):
        self.assertEqual(dice([1,1,0,0], [1,0,1,0]), 0.5)
        self.assertEqual(dice([1,0,1], [1,0,1]), 1.0)
        self.assertEqual(dice([1,0], [0,1]), 0.0)
        self.assertEqual(dice([0,0], [0,0]), 1.0)

    def test_invalid(self):
        for a,b in (([], []), ([1], []), ([1,2], [1,0]), ([True], [1]),
                    ([1.0], [1]), (["1"], [1])):
            with self.subTest(a=a,b=b), self.assertRaises(ValueError):
                dice(a,b)

if __name__ == "__main__":
    unittest.main()
