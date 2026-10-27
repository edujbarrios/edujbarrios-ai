import unittest
from top_k_logit_filter import top_k_logit_filter as filt


class TopKTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(filt([2, 5, 5, -1], 2), [False, True, True, False])
        self.assertEqual(filt([2, 5, 5, -1], 1), [False, True, False, False])
        self.assertEqual(filt([2, 5, 5, -1], 4), [True] * 4)
        self.assertEqual(filt([-2, -1, -3, -1], 2),
                         [False, True, False, True])
        self.assertEqual(filt([-3], 1), [True])

    def test_invalid(self):
        cases = [([], 1), ([1] * 4097, 1), ([1], 0),
                 ([1], 2), ([1], True), ([True], 1),
                 ([float('nan')], 1), ([float('inf')], 1),
                 ([1001], 1), ([1], 1.5), ('1', 1)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    filt(*args)


if __name__ == "__main__":
    unittest.main()
