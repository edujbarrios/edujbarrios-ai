import unittest
from sorted_lexicon_prefix_search import sorted_lexicon_prefix_search as search


class SortedLexiconTests(unittest.TestCase):
    def test_prefixes(self):
        words = ["ant", "antenna", "ape", "bat", "batch", "cat"]
        self.assertEqual(search(words, "an"), [0, 1])
        self.assertEqual(search(words, "ba"), [3, 4])
        self.assertEqual(search(words, "cat"), [5])
        self.assertEqual(search(words, "z"), [])
        self.assertEqual(search(words, ""), list(range(6)))
        self.assertEqual(search([], ""), [])
        self.assertEqual(search(["a"], "a"), [0])
        self.assertEqual(search(["a" * 512], "a" * 512), [0])

    def test_invalid(self):
        for words, prefix in [
            (["bat", "ant"], "a"), (["ant", "ant"], "an"),
            ([""], ""), (["Ant"], "a"), (["mañana"], "m"),
            (["a-b"], "a"), (["a" * 513], "a"),
            (["ant"], "A"), (["ant"], "a" * 513),
            (["ant"], None), (["ant", None], "a"),
        ]:
            with self.subTest(words=words, prefix=prefix), self.assertRaises(ValueError):
                search(words, prefix)


if __name__ == "__main__":
    unittest.main()
