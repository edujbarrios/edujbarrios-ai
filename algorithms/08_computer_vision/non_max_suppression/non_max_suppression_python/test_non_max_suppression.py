import math
import unittest
from non_max_suppression import non_max_suppression as nms


class NMSTests(unittest.TestCase):
    def setUp(self):
        self.boxes = [
            (0, 0, 2, 2, .9), (.5, .5, 2.5, 2.5, .8),
            (3, 3, 5, 5, .7), (0, 0, 2, 2, .9),
            (10, 10, 12, 12, .7)
        ]

    def test_reference_and_threshold(self):
        self.assertEqual(nms(self.boxes, .3), [0, 2, 4])
        self.assertEqual(nms(self.boxes, .4), [0, 1, 2, 4])
        self.assertEqual(nms([(0,0,1,1,1),(1,0,2,1,.8)], 0), [0, 1])
        self.assertEqual(nms([], 1), [])

    def test_invalid(self):
        cases = [
            ([(0,0,0,2,.5)], .5), ([(0,0,1,1,math.nan)], .5),
            ([(0,0,1000001,1,.5)], .5), (self.boxes, -0.1),
            (self.boxes, math.inf), (self.boxes, True),
            ([(0,0,1,1,True)], .5), ([(0,0,1)], .5),
            ([self.boxes[0]] * 257, .5),
        ]
        for boxes, threshold in cases:
            with self.subTest(threshold=threshold):
                with self.assertRaises(ValueError):
                    nms(boxes, threshold)


if __name__ == "__main__":
    unittest.main()
