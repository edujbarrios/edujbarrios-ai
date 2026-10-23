import unittest
from mean_reciprocal_rank import mean_reciprocal_rank as mrr

class MRRTests(unittest.TestCase):
    def test_values(self):
        self.assertAlmostEqual(mrr([1,2,0,4]),0.4375)
        self.assertEqual(mrr([0,0,0]),0.0)
        self.assertEqual(mrr([1]),1.0)
    def test_invalid(self):
        for ranks in ([],[-1],[1.5],[True],[1,"2"]):
            with self.subTest(ranks=ranks), self.assertRaises(ValueError):
                mrr(ranks)
if __name__=="__main__":
    unittest.main()
