import unittest

from byte_pair_encoding import (
    train_byte_pair_encoding as train, apply_byte_pair_merges as apply,
)


class BytePairEncodingTests(unittest.TestCase):
    def test_worked_example_and_roundtrip(self):
        corpus = ["abab", "abab"]
        trained = train(corpus, max_merges=2)
        self.assertEqual(trained["merges"], [[97, 98], [256, 256]])
        self.assertEqual(trained["token_ids"], [[257], [257]])
        self.assertEqual(apply("abab", trained["merges"]), [257])
        self.assertEqual(corpus, ["abab", "abab"])
        for text, ids in zip(corpus, trained["token_ids"]):
            self.assertEqual(apply(text, trained["merges"]), ids)

    def test_ties_overlaps_and_boundaries(self):
        tied = train(["ab", "cd", "ab", "cd"], 1)
        self.assertEqual(tied["merges"], [[97, 98]])
        overlap = train(["aaaa"], 2)
        self.assertEqual(overlap["merges"], [[97, 97]])
        self.assertEqual(overlap["token_ids"], [[256, 256]])
        self.assertEqual(apply("aaaa", overlap["merges"]), [256, 256])
        self.assertEqual(train(["a", "b"], 2)["merges"], [])
        self.assertEqual(train(["", "x"], 2)["token_ids"], [[], [120]])

    def test_unicode_and_zero_merge_budget(self):
        text = "πé猫"
        expected_bytes = list(text.encode("utf-8"))
        self.assertEqual(train([text], 0)["token_ids"], [expected_bytes])
        self.assertEqual(apply(text, []), expected_bytes)
        self.assertEqual(apply("", []), [])

    def test_invalid(self):
        for args in (
            ([], 1, 2), ([""], 1, 2), (["a", 2], 1, 2),
            (["a"], True, 2), (["a"], -1, 2), (["a"], 257, 2),
            (["a"], 1, 1), (["a"], 1, True),
            (["\ud800"], 1, 2), (None, 1, 2),
        ):
            with self.subTest(args=args), self.assertRaises(ValueError):
                train(*args)
        for text, merges in [
            (5, []), ("x", None), ("x", [(256, 1)]),
            ("x", [(97, True)]), ("x", [(97, 98), (97, 98)]),
            ("x", [[97]]), ("\ud800", []),
        ]:
            with self.subTest(text=text, merges=merges), self.assertRaises(ValueError):
                apply(text, merges)


if __name__ == "__main__":
    unittest.main()
