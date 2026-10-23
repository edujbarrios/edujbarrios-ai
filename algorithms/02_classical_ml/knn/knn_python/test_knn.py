import unittest
from knn import knn


class KNNTests(unittest.TestCase):
    def setUp(self):
        self.samples = [[0, 0], [5, 5], [2, 1], [0, 2]]

    def test_tie_and_order(self):
        self.assertEqual(knn(self.samples, [0, 1], 2), [0, 3])
        self.assertEqual(knn(self.samples, [0, 1], 4), [0, 3, 2, 1])

    def test_invalid(self):
        for samples, query, k in (([], [0], 1), (self.samples, [], 1),
                                  (self.samples, [0, 1], 0),
                                  (self.samples, [0, 1], 5),
                                  (self.samples, [0, 1], True),
                                  ([[0], [0, 1]], [0], 1),
                                  ([[float("nan")]], [0], 1),
                                  ([[1e308]], [-1e308], 1)):
            with self.subTest(samples=samples, query=query, k=k):
                with self.assertRaises(ValueError):
                    knn(samples, query, k)


if __name__ == "__main__":
    unittest.main()
