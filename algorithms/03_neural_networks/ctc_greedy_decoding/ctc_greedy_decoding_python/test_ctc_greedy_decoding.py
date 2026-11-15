import math
import unittest
from ctc_greedy_decoding import ctc_greedy_decoding as decode


class CTCGreedyTests(unittest.TestCase):
    def test_collapse_order_and_path_score(self):
        rows = [[0.1, 0.9], [0.1, 0.9], [0.9, 0.1], [0.1, 0.9]]
        result = decode(rows)
        self.assertEqual(result["token_ids"], [1, 1])
        self.assertEqual(result["alignment"], [1, 1, 0, 1])
        self.assertAlmostEqual(result["path_log_probability"], 4 * math.log(0.9))
        self.assertEqual(rows, [[0.1, 0.9], [0.1, 0.9],
                                [0.9, 0.1], [0.1, 0.9]])
        self.assertEqual(decode([[0.1, 0.9], [0.1, 0.9]])["token_ids"], [1])
        self.assertEqual(decode([[1.0, 0.0]])["token_ids"], [])

    def test_custom_blank_and_tie(self):
        result = decode([[.5, .5, 0], [0, 1, 0], [0, 0, 1]], blank_id=2)
        self.assertEqual(result["alignment"], [0, 1, 2])
        self.assertEqual(result["token_ids"], [0, 1])
        self.assertEqual(decode([[.5, .5], [.5, .5]])["alignment"], [0, 0])
        # Best frame path need not produce the most probable output sequence.
        self.assertEqual(decode([[.6, .4], [.6, .4]])["token_ids"], [])

    def test_invalid(self):
        for rows, blank in (
            ([], 0), ([[1]], 0), ([[.5, .4]], 0),
            ([[.5, .5], [.4]], 0), ([[True, 0]], 0),
            ([[math.nan, .5]], 0), ([[math.inf, .5]], 0),
            ([[1.1, -.1]], 0), ([[.5, .5]], 2),
            ([[.5, .5]], True), (None, 0),
        ):
            with self.subTest(rows=rows, blank=blank), self.assertRaises(ValueError):
                decode(rows, blank)


if __name__ == "__main__":
    unittest.main()
