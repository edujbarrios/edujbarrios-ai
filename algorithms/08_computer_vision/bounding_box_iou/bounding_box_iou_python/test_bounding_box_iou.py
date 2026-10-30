import math
import unittest
from bounding_box_iou import bounding_box_iou as iou


class BoundingBoxIoUTests(unittest.TestCase):
    def test_partial_overlap(self):
        self.assertAlmostEqual(iou([0, 0, 2, 2], [1, 1, 3, 3]), 1 / 7)

    def test_edges(self):
        box = [0, 0, 2, 2]
        self.assertEqual(iou(box, box), 1.0)
        self.assertEqual(iou(box, [4, 4, 5, 5]), 0.0)
        self.assertEqual(iou(box, [2, 0, 3, 1]), 0.0)
        self.assertEqual(iou(box, [-2, -2, 0, 0]), 0.0)

    def test_invalid(self):
        box = [0, 0, 2, 2]
        invalid = ([0, 0, 1], [0, 0, 0, 1], [0, 0, 1, math.inf],
                   [0, math.nan, 2, 2], [0, 0, 1000001, 2],
                   [0, True, 2, 2], [0, "1", 2, 2])
        for other in invalid:
            with self.subTest(other=other), self.assertRaises(ValueError):
                iou(box, other)


if __name__ == "__main__":
    unittest.main()
