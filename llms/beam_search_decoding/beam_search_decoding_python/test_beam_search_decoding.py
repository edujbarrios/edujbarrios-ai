"""Determinism, prefix dependence, EOS, and masked-score edge cases."""

import math
import unittest
from beam_search_decoding import beam_search_decoding as decode


class BeamSearchTests(unittest.TestCase):
    def test_prefix_aware_search_recovers_non_greedy_path(self):
        def scores(prefix):
            if not prefix:
                return [-0.1, -0.2]
            return [-5, -5] if prefix == (0,) else [-0.01, -0.02]

        best = decode(scores, 2, beam_width=2, max_new_tokens=2)
        self.assertEqual([path for path, _ in best], [[1, 0], [1, 1]])
        self.assertAlmostEqual(best[0][1], -0.21)
        self.assertEqual(decode(scores, 2, beam_width=1, max_new_tokens=2)[0][0],
                         [0, 0])
        self.assertEqual(decode(scores, 2, beam_width=2, max_new_tokens=0),
                         [([], 0.0)])

    def test_eos_and_ties(self):
        calls = []
        def scores(prefix):
            calls.append(prefix)
            return [-0.2, -0.4, -0.1] if not prefix else [-3, -3, -0.05]

        result = decode(scores, 3, beam_width=2, max_new_tokens=5,
                        eos_token_id=2)
        self.assertEqual([path for path, _ in result], [[2], [0, 2]])
        self.assertEqual(calls, [(), (0,)])
        tied = decode(lambda _: [-1, -1], 2, beam_width=2, max_new_tokens=1)
        self.assertEqual([path for path, _ in tied], [[0], [1]])
        self.assertEqual(decode(lambda _: [-math.inf, -1], 2, 1, 1)[0][0], [1])

    def test_invalid(self):
        for kwargs in ({"vocabulary_size": 0}, {"vocabulary_size": True},
                       {"beam_width": 0}, {"beam_width": True},
                       {"max_new_tokens": -1}, {"max_new_tokens": True},
                       {"eos_token_id": 2}, {"eos_token_id": True}):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                params = {"vocabulary_size": 2, "beam_width": 1, "max_new_tokens": 1}
                params.update(kwargs)
                decode(lambda _: [-1, -1], **params)
        for callback in (lambda _: [0], lambda _: [math.nan, -1],
                         lambda _: [math.inf, -1], lambda _: [0.1, -1],
                         lambda _: [True, -1], lambda _: [-math.inf] * 2):
            with self.subTest(callback=callback), self.assertRaises(ValueError):
                decode(callback, 2, 1, 1)
        with self.assertRaises(ValueError):
            decode(None, 2, 1, 1)
        with self.assertRaises(ValueError):
            decode(lambda _: [-1e308], 1, 1, 2)


if __name__ == "__main__":
    unittest.main()
