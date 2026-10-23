import unittest
from dense_forward import dense_forward as forward


class DenseForwardTests(unittest.TestCase):
    def test_known_output(self):
        self.assertEqual(forward([2,-1],[[1,1],[-1,2]],[0,1]), [1.0,0.0])
        self.assertEqual(forward([1],[[3]],[2]), [5.0])
        self.assertEqual(forward([-1],[[3]],[2]), [0.0])

    def test_invalid(self):
        for x,w,b in (([], [[1]], [0]),
                      ([1], [], []),
                      ([1,2], [[1]], [0]),
                      ([1,2], [[1,2]], [0,0]),
                      ([1], [[float("nan")]], [0]),
                      ([1], [[1]], [float("inf")]),
                      ([1e308], [[1e308]], [0]),
                      ([True], [[1]], [0])):
            with self.subTest(x=x,w=w,b=b), self.assertRaises(ValueError):
                forward(x,w,b)


if __name__ == "__main__":
    unittest.main()
