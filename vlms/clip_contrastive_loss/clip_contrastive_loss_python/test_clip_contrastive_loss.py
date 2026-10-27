import math
import unittest
from clip_contrastive_loss import clip_contrastive_loss as loss


class ClipLossTests(unittest.TestCase):
    def test_reference_examples(self):
        images = [1, 0, 0, 1]
        self.assertAlmostEqual(loss(images, images, 2, 2, 1),
                               math.log1p(math.exp(-1)), places=12)
        self.assertAlmostEqual(loss(images, [0, 1, 1, 0], 2, 2, 1),
                               math.log1p(math.exp(1)), places=12)
        self.assertAlmostEqual(loss([1, 0, 1, 0], [1, 0, 1, 0], 2, 2, 1),
                               math.log(2), places=12)
        self.assertLess(loss(images, images, 2, 2, 100), 1e-12)
        self.assertAlmostEqual(loss([2, 0, 0, 3], images, 2, 2, 1),
                               math.log1p(math.exp(-1)), places=12)

    def test_bad_inputs(self):
        images = [1, 0, 0, 1]
        cases = [
            (images, images, 1, 2, 1), (images, images, 2, 0, 1),
            (images, images, 2, 129, 1), (images, images, 2, 2, 0),
            (images, images, 2, 2, 101), (images, images, 2, 2, math.nan),
            (images, [0, 0, 0, 1], 2, 2, 1),
            (images, [1, 0, math.inf, 1], 2, 2, 1),
            (images, [1, 0, 1001, 1], 2, 2, 1),
            (images, [1, 0, 1], 2, 2, 1),
            (images, [True, 0, 0, 1], 2, 2, 1),
            (images, images, True, 2, 1),
        ]
        for args in cases:
            with self.subTest(args=str(args)[:80]):
                with self.assertRaises(ValueError):
                    loss(*args)


if __name__ == "__main__":
    unittest.main()
