import unittest

from hamming_distance import hamming_distance


class HammingDistanceTests(unittest.TestCase):
    def test_substitutions(self):
        self.assertEqual(hamming_distance(bytes([1, 0, 1, 1]),
                                          bytes([1, 1, 0, 1])), 2)
        self.assertEqual(hamming_distance(b"abc", b"abc"), 0)

    def test_boundaries(self):
        self.assertEqual(hamming_distance(b"", b""), 0)
        self.assertEqual(hamming_distance(bytes([0]), bytes([255])), 1)
        self.assertEqual(hamming_distance(bytearray(b"hi"), b"ho"), 1)

    def test_invalid(self):
        for a, b in ((b"a", b""), ([1], [1]), ("ab", "ab"),
                     (None, b"")):
            with self.subTest(a=a, b=b), self.assertRaises(ValueError):
                hamming_distance(a, b)


if __name__ == "__main__":
    unittest.main()
