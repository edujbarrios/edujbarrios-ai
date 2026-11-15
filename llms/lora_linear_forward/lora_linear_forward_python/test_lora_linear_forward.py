"""LoRA adapter correctness, baseline equivalence, and shape validation."""

import math
import unittest
from lora_linear_forward import lora_linear_forward as forward


class LoRALinearTests(unittest.TestCase):
    def test_worked_example_and_immutability(self):
        x, w, a, b = [2, 3], [[1, 0], [0, 1]], [[1, 1]], [[2], [-1]]
        self.assertEqual(forward(x, w, a, b), [12.0, -2.0])
        self.assertEqual(forward(x, w, a, b, alpha=2, bias=[1, 1]),
                         [23.0, -6.0])
        self.assertEqual((x, w, a, b),
                         ([2, 3], [[1, 0], [0, 1]], [[1, 1]], [[2], [-1]]))

    def test_zero_adapter_and_multiple_ranks(self):
        self.assertEqual(forward([1, 2], [[2, 3]], [[1, 0]], [[0]], bias=[4]),
                         [12.0])
        self.assertEqual(forward([1, 1], [[0, 0]],
                                 [[1, 0], [0, 1]], [[2, 4]], alpha=2),
                         [6.0])
        self.assertEqual(forward([0], [[2]], [[3]], [[4]], alpha=1), [0.0])

    def test_invalid(self):
        valid = ([1, 2], [[1, 0]], [[1, 0]], [[1]])
        invalid = [
            ([], [[1]], [[1]], [[1]]),
            ([1, 2], [[1]], [[1, 2]], [[1]]),
            ([1, 2], [[1, 0]], [[1]], [[1]]),
            ([1, 2], [[1, 0]], [], [[1]]),
            ([1, 2], [[1, 0]], [[1, 0]], [[1, 2]]),
            ([1, 2], [[1, 0]], [[1, 0]], [[1], [2]]),
            ([True, 2], [[1, 0]], [[1, 0]], [[1]]),
            ([1, math.nan], [[1, 0]], [[1, 0]], [[1]]),
            ([1, 2], [[math.inf, 0]], [[1, 0]], [[1]]),
        ]
        for args in invalid:
            with self.subTest(args=args), self.assertRaises(ValueError):
                forward(*args)
        for kwargs in ({"alpha": 0}, {"alpha": -1}, {"alpha": math.inf},
                       {"alpha": True}, {"bias": [1, 2]},
                       {"bias": [math.nan]}):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                forward(*valid, **kwargs)


if __name__ == "__main__":
    unittest.main()
