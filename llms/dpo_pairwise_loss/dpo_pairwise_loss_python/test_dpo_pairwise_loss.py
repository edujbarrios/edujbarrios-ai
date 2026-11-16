import math
import unittest
from dpo_pairwise_loss import dpo_pairwise_loss as loss


class DPOLossTests(unittest.TestCase):
    def test_example_and_ties(self):
        result = loss([-0.1], [-1], [-0.4], [-0.6], beta=2)
        expected = math.log1p(math.exp(-1.4))
        self.assertAlmostEqual(result["mean_loss"], expected)
        self.assertEqual(result["preference_accuracy"], 1.0)
        self.assertAlmostEqual(loss([-1], [-1], [-2], [-2])["mean_loss"], math.log(2))
        self.assertEqual(loss([-1], [-1], [-2], [-2])["preference_accuracy"], 0.0)

    def test_batch_stability_and_immutability(self):
        chosen, rejected = [-0.1, -1.0, -1_000_000], [-2.0, -0.1, 0]
        refs = [-0.5, -0.5, -0.5]
        result = loss(chosen, rejected, refs, refs, beta=100)
        self.assertEqual(result["preference_accuracy"], 1/3)
        self.assertEqual(len(result["losses"]), 3)
        self.assertTrue(all(math.isfinite(value) and value >= 0 for value in result["losses"]))
        self.assertAlmostEqual(result["mean_loss"],
                               math.fsum(result["losses"]) / 3)
        self.assertEqual(chosen, [-0.1, -1, -1_000_000])
        self.assertEqual(rejected, [-2, -0.1, 0])
        positive = loss([0], [-1_000_000], [0], [0], beta=100)
        self.assertGreaterEqual(positive["mean_loss"], 0)
        self.assertLess(positive["mean_loss"], 1e-10)

    def test_invalid(self):
        valid = ([-1], [-2], [-3], [-4])
        for kwargs in ({"beta": True}, {"beta": 0}, {"beta": -1},
                       {"beta": 101}, {"beta": math.inf}, {"beta": math.nan}):
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                loss(*valid, **kwargs)
        invalid = (
            ([], [], [], []),
            ([-1], [-2, -3], [-3], [-4]),
            ([True], [-1], [-1], [-1]),
            ([math.nan], [-1], [-1], [-1]),
            ([math.inf], [-1], [-1], [-1]),
            ([0.1], [-1], [-1], [-1]),
            ([-1_000_001], [-1], [-1], [-1]),
            (["-1"], [-1], [-1], [-1]),
            (None, [-1], [-1], [-1]),
        )
        for args in invalid:
            with self.subTest(args=args), self.assertRaises(ValueError):
                loss(*args)


if __name__ == "__main__":
    unittest.main()
