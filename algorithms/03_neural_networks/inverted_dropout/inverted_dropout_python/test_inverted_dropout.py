import unittest
from inverted_dropout import inverted_dropout as dropout


class InvertedDropoutTests(unittest.TestCase):
    def test_forward(self):
        self.assertEqual(dropout([2.0, -4.0, 0.0], [1, 0, 1], 0.5),
                         [4.0, 0.0, 0.0])
        self.assertEqual(dropout([3.0, -2.0], [1, 1], 0.0), [3.0, -2.0])
        self.assertEqual(dropout([1.0e308], [0], 0.99), [0.0])
        self.assertEqual(dropout([], [], 0.5), [])
        self.assertEqual(dropout([0], [1], 0.99999999), [0.0])

    def test_invalid(self):
        for values, mask, p in [
            ([1], [], 0.5), ([1], [2], 0.5), ([1], [True], 0.5),
            ([float("nan")], [0], 0.5), ([float("inf")], [1], 0.5),
            ([1], [1], -0.1), ([1], [1], 1.0), ([1], [1], float("nan")),
            ([1.0e308], [1], 0.99), ([True], [1], 0.5),
            ([1], [1], True),
        ]:
            with self.subTest(values=values, mask=mask, p=p), self.assertRaises(ValueError):
                dropout(values, mask, p)


if __name__ == "__main__":
    unittest.main()
