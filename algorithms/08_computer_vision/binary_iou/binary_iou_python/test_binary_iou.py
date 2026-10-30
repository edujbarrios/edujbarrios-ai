import unittest

from binary_iou import binary_iou


class BinaryIoUTests(unittest.TestCase):
    def test_overlap(self):
        self.assertAlmostEqual(binary_iou([1, 1, 0, 0], [1, 0, 1, 0]), 1 / 3)

    def test_identical_and_empty_foreground(self):
        self.assertEqual(binary_iou([1, 0, 1], [1, 0, 1]), 1.0)
        self.assertEqual(binary_iou([0, 0, 0], [0, 0, 0]), 1.0)

    def test_disjoint(self):
        self.assertEqual(binary_iou([1, 0], [0, 1]), 0.0)

    def test_invalid(self):
        cases = [([], []), ([0], []), ([1, 2], [1, 0]),
                 ([True], [1]), ([1.0], [1]), (["0"], [0])]
        for prediction, target in cases:
            with self.subTest(prediction=prediction, target=target):
                with self.assertRaises(ValueError):
                    binary_iou(prediction, target)


if __name__ == "__main__":
    unittest.main()
