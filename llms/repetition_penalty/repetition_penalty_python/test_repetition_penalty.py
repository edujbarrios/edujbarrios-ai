import unittest
from repetition_penalty import repetition_penalty


class RepetitionPenaltyTests(unittest.TestCase):
    def test_reference(self):
        logits = [2, -2, 0, 1]
        self.assertEqual(repetition_penalty(logits, [0, 1, 1, 2], 2),
                         [1, -4, 0, 1])
        self.assertEqual(logits, [2, -2, 0, 1])
        self.assertEqual(repetition_penalty(logits, [0, 1, 1, 2], 1), logits)
        self.assertEqual(repetition_penalty(logits, [], 10), logits)
        self.assertEqual(repetition_penalty([1], [0], 10), [.1])

    def test_invalid(self):
        x = [2, -2, 0, 1]
        cases = [(x, [4], 2), (x, [-1], 2), (x, [True], 2),
                 (x, [0.1], 2), (x, [0]*16385, 2),
                 ([], [], 2), ([1]*4097, [], 2),
                 (x, [], 0.5), (x, [], 11), (x, [], float("nan")),
                 ([True], [], 2), ([float("inf")], [], 2),
                 ([1001], [], 2), (x, "123", 2)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    repetition_penalty(*args)


if __name__ == "__main__":
    unittest.main()
