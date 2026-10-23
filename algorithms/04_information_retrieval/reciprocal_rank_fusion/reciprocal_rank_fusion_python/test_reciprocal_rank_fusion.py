import unittest
from reciprocal_rank_fusion import reciprocal_rank_fusion


class RRFTests(unittest.TestCase):
    def test_example(self):
        self.assertEqual(reciprocal_rank_fusion([10,20,30],[30,10,40]),
                         [10,30,20,40])

    def test_ties(self):
        self.assertEqual(reciprocal_rank_fusion([2,1],[1,2]), [1,2])

    def test_one_empty(self):
        self.assertEqual(reciprocal_rank_fusion([10,20,30],[]),
                         [10,20,30])

    def test_invalid(self):
        for a, b in (([],[]), ([1,1],[]), ([-1],[]),
                     ([2_147_483_648],[]), ([True],[]),
                     (["1"],[])):
            with self.subTest(a=a,b=b), self.assertRaises(ValueError):
                reciprocal_rank_fusion(a,b)


if __name__ == "__main__":
    unittest.main()
