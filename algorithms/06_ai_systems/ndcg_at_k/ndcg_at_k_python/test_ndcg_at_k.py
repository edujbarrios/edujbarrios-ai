import unittest
from ndcg_at_k import ndcg_at_k


class NDCGTests(unittest.TestCase):
    def test_ideal_and_nonideal(self):
        self.assertAlmostEqual(ndcg_at_k([3, 2, 1, 0], 3), 1.0)
        self.assertGreater(ndcg_at_k([3, 2, 0, 1], 3), 0.9)
        self.assertLess(ndcg_at_k([3, 2, 0, 1], 3), 1.0)

    def test_zero_and_invalid(self):
        self.assertEqual(ndcg_at_k([0, 0, 0], 2), 0.0)
        for grades, k in (([], 1), ([1], 0), ([1], 2),
                          ([1, 2, 21], 1), ([1, True], 1), ([0.5], 1)):
            with self.subTest(grades=grades, k=k), self.assertRaises(ValueError):
                ndcg_at_k(grades, k)


if __name__ == "__main__":
    unittest.main()
