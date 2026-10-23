import unittest
from jaccard_similarity import jaccard_similarity as jaccard

class JaccardTests(unittest.TestCase):
    def test_overlap_and_empty(self):
        self.assertEqual(jaccard([1,1,2,3],[2,3,4]), 0.5)
        self.assertEqual(jaccard([],[]), 1.0)
        self.assertEqual(jaccard([],[3]), 0.0)
        self.assertEqual(jaccard([-5,3],[3,-5]), 1.0)
        self.assertEqual(jaccard([-2**31, 2**31-1],[-2**31,2**31-1]),1.0)
    def test_invalid(self):
        for item in (True, 1.5, "1", 2**31, -2**31-1):
            with self.subTest(item=item), self.assertRaises(ValueError):
                jaccard([item],[])
if __name__=="__main__":
    unittest.main()
