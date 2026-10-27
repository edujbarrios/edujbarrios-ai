import math
import unittest
from clip_zero_shot_classification import clip_zero_shot_classification as classify


class ZeroShotTests(unittest.TestCase):
    def test_expected_probabilities(self):
        expected = math.exp(1) / (math.exp(1) + 1)
        self.assertAlmostEqual(classify([1,0],[1,0,0,1],2,2,1)[0],expected)
        self.assertAlmostEqual(classify([0,1],[1,0,0,1],2,2,1)[1],expected)
        self.assertEqual(classify([1,0],[1,0,1,0],2,2,1),[.5,.5])
        self.assertAlmostEqual(classify([2,0],[3,0,0,5],2,2,1)[0],expected)
        self.assertGreater(classify([1,0],[1,0,0,1],2,2,100)[0],.99999)
        self.assertAlmostEqual(sum(classify([1,0],[1,0,0,1],2,2,1)),1)

    def test_invalid(self):
        image=[1,0]
        texts=[1,0,0,1]
        cases=[
            (image,texts,1,2,1),(image,texts,65,2,1),
            (image,texts,2,0,1),(image,texts,2,129,1),
            (image,texts,2,2,0),(image,texts,2,2,float("nan")),
            ([0,0],texts,2,2,1),(image,[0,0,0,1],2,2,1),
            (image,[1,0,float("inf"),1],2,2,1),
            (image,[1,0,1001,1],2,2,1),
            (image,[1,0,1],2,2,1),(image,[True,0,0,1],2,2,1),
            (image,texts,True,2,1)
        ]
        for args in cases:
            with self.subTest(args=str(args)[:80]):
                with self.assertRaises(ValueError):
                    classify(*args)


if __name__ == "__main__":
    unittest.main()
