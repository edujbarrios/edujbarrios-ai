import itertools
import math
import unittest

from ctc_loss import ctc_loss


def enumerate_paths(rows, targets, blank=0):
    total = 0.0
    for path in itertools.product(range(len(rows[0])), repeat=len(rows)):
        collapsed = []
        previous = None
        for token in path:
            if token != blank and token != previous:
                collapsed.append(token)
            previous = token
        if collapsed == list(targets):
            total += math.prod(row[token] for row, token in zip(rows, path))
    return -math.log(total) if total else math.inf


class CTCLossTests(unittest.TestCase):
    def test_known_cases(self):
        self.assertAlmostEqual(ctc_loss([[.5, .5], [.5, .5]], [1]),
                               -math.log(.75))
        self.assertEqual(ctc_loss([[0, 1], [1, 0], [0, 1]], [1, 1]), 0.0)
        self.assertEqual(ctc_loss([[.5, .5], [.5, .5]], [1, 1]), math.inf)
        self.assertAlmostEqual(ctc_loss([[1, 0], [.5, .5]], []), -math.log(.5))
        self.assertEqual(ctc_loss([[0, 1]], [1]), 0.0)

    def test_enumerated_alignment_oracle(self):
        rows = [[.2, .5, .3], [.3, .4, .3], [.4, .2, .4], [.3, .3, .4]]
        rows_before = [r[:] for r in rows]
        for target in ([], [1], [2], [1, 1], [1, 2], [2, 1], [1, 2, 1]):
            with self.subTest(target=target):
                self.assertAlmostEqual(ctc_loss(rows, target),
                                       enumerate_paths(rows, target), places=12)
        self.assertEqual(rows, rows_before)
        custom = [[.5, .2, .3], [.4, .5, .1]]
        for target in ([], [0], [1], [0, 1]):
            self.assertAlmostEqual(ctc_loss(custom, target, blank_id=2),
                                   enumerate_paths(custom, target, 2), places=12)

    def test_invalid_inputs(self):
        for rows, labels, blank in [
            ([], [], 0), ([[1]], [], 0),
            ([[.5, .5], [.2]], [1], 0),
            ([[.5, .4]], [1], 0), ([[1.1, -.1]], [1], 0),
            ([[math.nan, .5]], [1], 0), ([[True, 0]], [1], 0),
            ([[.5, .5]], [0], 0), ([[.5, .5]], [True], 0),
            ([[.5, .5]], [2], 0), ([[.5, .5]], [1], True),
            ([[.5, .5]], [1], 2), (None, [], 0),
            ([[.5, .5]], None, 0),
        ]:
            with self.subTest(rows=rows, labels=labels, blank=blank):
                with self.assertRaises(ValueError):
                    ctc_loss(rows, labels, blank)


if __name__ == "__main__":
    unittest.main()
