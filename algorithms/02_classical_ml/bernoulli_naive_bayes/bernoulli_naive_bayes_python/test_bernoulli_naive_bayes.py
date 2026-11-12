import unittest
from bernoulli_naive_bayes import bernoulli_naive_bayes as predict


class BernoulliNaiveBayesTests(unittest.TestCase):
    def test_examples(self):
        samples = [1, 0, 1, 1, 0, 1, 0, 0]
        labels = [0, 0, 1, 1]
        a, p = predict(samples, labels, 4, 2, [1, 0])
        self.assertEqual(a, 0)
        self.assertAlmostEqual(p, 0.25)
        a, p = predict(samples, labels, 4, 2, [0, 1])
        self.assertEqual(a, 1)
        self.assertAlmostEqual(p, 0.75)
        self.assertEqual(predict([0, 0], [0, 1], 2, 1, [0]), (0, 0.5))
        self.assertEqual(predict([0, 0, 0], [0, 0, 1], 3, 1, [0])[0], 0)
        large = [0] * (64 * 16) + [1] * (64 * 16)
        a, p = predict(large, [0] * 64 + [1] * 64, 128, 16, [1] * 16)
        self.assertEqual(a, 1)
        self.assertGreater(p, 0.999)

    def test_invalid(self):
        samples = [1, 0, 1, 1, 0, 1, 0, 0]
        labels = [0, 0, 1, 1]
        cases = [
            (samples, labels, 1, 2, [1, 0]),
            (samples, labels, 4, 0, [1, 0]),
            (samples, labels, 4, 17, [1, 0]),
            (samples, labels, True, 2, [1, 0]),
            (samples[:-1], labels, 4, 2, [1, 0]),
            (samples, labels[:-1], 4, 2, [1, 0]),
            (samples, labels, 4, 2, [1]),
            (samples, [0, 0, 0, 0], 4, 2, [1, 0]),
            (samples, [0, 0, 1, 2], 4, 2, [1, 0]),
            ([1, 0, 1, 2, 0, 1, 0, 0], labels, 4, 2, [1, 0]),
            (samples, labels, 4, 2, [1, True]),
            (samples, labels, 4, 2, [1, -1]),
            (None, labels, 4, 2, [1, 0]),
        ]
        for args in cases:
            with self.subTest(case=args), self.assertRaises(ValueError):
                predict(*args)


if __name__ == "__main__":
    unittest.main()
