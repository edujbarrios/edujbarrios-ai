import unittest
from minhash_signature import minhash_signature as signature


class MinHashTests(unittest.TestCase):
    def test_known_signatures(self):
        self.assertEqual(signature([1, 2, 3], [1, 3], [0, 7]), [1, 10])
        self.assertEqual(signature([3, 1, 2, 1], [1, 3], [0, 7]), [1, 10])
        self.assertEqual(signature([], [1, 3], [0, 7]), [65521, 65521])
        self.assertEqual(signature([65520], [65520], [65520]), [0])
        self.assertEqual(signature([0], [1], [13]), [13])

    def test_invalid(self):
        cases = [
            ([1], [], []), ([1], [1], []), ([-1], [1], [0]),
            ([65521], [1], [0]), ([1], [0], [0]),
            ([1], [65521], [0]), ([1], [1], [-1]),
            ([1], [1], [65521]), ([1.5], [1], [0]),
            ([1], [1.0], [0]), ([True], [1], [0]),
            ([1], [1], [False]),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                signature(*args)


if __name__ == "__main__":
    unittest.main()
