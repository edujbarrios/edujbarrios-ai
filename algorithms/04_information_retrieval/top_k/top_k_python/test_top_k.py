import unittest
from top_k import top_k

class TopKTests(unittest.TestCase):
    def test_ranking(self):
        self.assertEqual(top_k([0.8, 0.9, 0.9, -1.0], 3), [1,2,0])
        self.assertEqual(top_k([1,1,1], 2), [0,1])
        self.assertEqual(top_k([2,-4], 2), [0,1])

    def test_invalid(self):
        for scores,k in (([],1),([1],0),([1],2),
                         ([0.1,float("nan")],1),
                         ([float("inf")],1),
                         ([True],1),([1],True)):
            with self.subTest(scores=scores,k=k):
                with self.assertRaises(ValueError):
                    top_k(scores,k)
if __name__=="__main__":
    unittest.main()
