import unittest
from no_repeat_ngram import no_repeat_ngram


class NoRepeatNgramTests(unittest.TestCase):
    def test_unigram(self):
        self.assertEqual(no_repeat_ngram([1, 2, 1], 4, 1),
                         [False, True, True, False])

    def test_bigram(self):
        self.assertEqual(no_repeat_ngram([1, 2, 1, 2], 5, 2),
                         [False, True, False, False, False])

    def test_trigram_and_overlap(self):
        self.assertEqual(no_repeat_ngram([1, 2, 3, 1, 2], 5, 3),
                         [False, False, False, True, False])
        self.assertEqual(no_repeat_ngram([1, 1, 1], 3, 2),
                         [False, True, False])

    def test_short(self):
        self.assertEqual(no_repeat_ngram([], 3, 1), [False] * 3)
        self.assertEqual(no_repeat_ngram([0, 1], 3, 3), [False] * 3)

    def test_invalid(self):
        for args in [([0], 0, 1), ([0], 4097, 1), ([0], 2, 0),
                     ([0], 2, 33), ([2], 2, 1), ([-1], 2, 1),
                     ([True], 2, 1), ([0.5], 2, 1), ("01", 2, 1),
                     ([0] * 16385, 2, 1), ([], True, 1), ([], 2, True)]:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    no_repeat_ngram(*args)


if __name__ == "__main__":
    unittest.main()
