import math
import unittest

from hough_line_transform import hough_line_transform as hough


class HoughLineTests(unittest.TestCase):
    def test_vertical_horizontal_and_diagonal(self):
        image = [[0, 1, 0], [0, 1, 0], [0, 1, 0]]
        self.assertEqual(hough(image, angles=[0, 90], top_k=3),
                         [(0, 1.0, 3), (90, 0.0, 1), (90, 1.0, 1)])
        self.assertEqual(image, [[0, 1, 0], [0, 1, 0], [0, 1, 0]])
        horizontal = [[0, 0, 0], [0, 0, 0], [1, 1, 1]]
        self.assertEqual(hough(horizontal, angles=[90], top_k=1),
                         [(90, 2.0, 3)])
        diagonal = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
        self.assertEqual(hough(diagonal, angles=[135], top_k=1),
                         [(135, 0.0, 3)])

    def test_empty_and_ties(self):
        self.assertEqual(hough([[0, 0], [0, 0]]), [])
        self.assertEqual(hough([[1, 0], [0, 1]], angles=[0], top_k=2),
                         [(0, 0.0, 1), (0, 1.0, 1)])
        self.assertEqual(hough([[1]], angles=[0, 90], top_k=2),
                         [(0, 0.0, 1), (90, 0.0, 1)])
        self.assertEqual(hough([[1, 1]], angles=[0], rho_step=0.5, top_k=2),
                         [(0, 0.0, 1), (0, 1.0, 1)])

    def test_invalid(self):
        for image in ([], [[]], [[0], [0, 1]], [[True]], [[1.0]],
                      [[-1]], [[2]], [[math.nan]], None, [[0] * 129]):
            with self.subTest(image=image), self.assertRaises(ValueError):
                hough(image)
        for kwargs in (
            {"angles": []}, {"angles": [0, 0]}, {"angles": [180]},
            {"angles": [-1]}, {"angles": [math.nan]},
            {"angles": [True]}, {"angles": "0"},
            {"rho_step": 0}, {"rho_step": 1e-4},
            {"rho_step": math.inf}, {"rho_step": True},
            {"top_k": 0}, {"top_k": True}, {"top_k": 1025},
        ):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                hough([[1]], **kwargs)


if __name__ == "__main__":
    unittest.main()
