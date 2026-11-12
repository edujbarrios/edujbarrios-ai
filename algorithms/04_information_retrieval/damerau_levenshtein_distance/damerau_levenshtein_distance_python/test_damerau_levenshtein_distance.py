import unittest
from damerau_levenshtein_distance import damerau_levenshtein_distance as dl


class DamerauLevenshteinTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(dl([67, 65], [65, 66, 67]), 2)
        self.assertEqual(dl([65, 66, 67], [67, 65]), 2)
        self.assertEqual(dl([1, 2], [2, 1]), 1)
        self.assertEqual(dl([1, 1, 2], [1, 2, 1]), 1)
        self.assertEqual(dl([0, 255], [255, 0]), 1)
        self.assertEqual(dl([1, 2, 3], [1, 4, 3]), 1)
        self.assertEqual(dl([1, 2, 3], [1, 2, 3]), 0)
        self.assertEqual(dl([], [0, 255]), 2)
        self.assertEqual(dl([0, 255], []), 2)
        self.assertEqual(dl([], []), 0)
        self.assertEqual(dl([0] * 256, [0] * 256), 0)
        self.assertEqual(dl([0] * 256, [1] * 256), 256)

    def test_invalid(self):
        for bad in ([256], [-1], [1.1], [True], ["1"], [None],
                    list(range(257)), None):
            with self.subTest(bad=type(bad)), self.assertRaises(ValueError):
                dl(bad, [])
            with self.subTest(bad=type(bad)), self.assertRaises(ValueError):
                dl([], bad)


if __name__ == "__main__":
    unittest.main()
