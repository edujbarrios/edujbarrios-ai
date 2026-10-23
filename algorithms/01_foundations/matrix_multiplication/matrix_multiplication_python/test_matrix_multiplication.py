import unittest
from matrix_multiplication import matrix_multiplication


class MatrixMultiplicationTests(unittest.TestCase):
    def test_rectangular(self):
        self.assertEqual(matrix_multiplication([[1, 2, 3], [4, 5, 6]],
                          [[7, 8], [9, 10], [11, 12]]),
                         [[58.0, 64.0], [139.0, 154.0]])

    def test_scalar(self):
        self.assertEqual(matrix_multiplication([[-3]], [[2]]), [[-6.0]])

    def test_invalid(self):
        for a, b in (([], [[1]]), ([[1]], []),
                     ([[1, 2], [3]], [[4], [5]]),
                     ([[1, 2]], [[3]]),
                     ([[float("nan")]], [[1]]),
                     ([[1]], [[float("inf")]]),
                     ([[1e308]], [[1e308]]),
                     ([[True]], [[1]])):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                matrix_multiplication(a, b)


if __name__ == "__main__":
    unittest.main()
