import unittest
from recall_at_k import recall_at_k

class RecallAtKTests(unittest.TestCase):
    def test_results(self):
        self.assertAlmostEqual(recall_at_k([1,0,1,0], 3, 3), 2/3)
        self.assertAlmostEqual(recall_at_k([1,0,1,0], 1, 3), 1/3)
        self.assertEqual(recall_at_k([0,0], 2, 5), 0.0)

    def test_invalid(self):
        for labels,k,total in (([],1,3),([1,0],0,3),([1,0],3,3),
                               ([1,0],1,0),([1,0,1],2,1),
                               ([1,0,2],1,3),([1,0,True],1,3),
                               ([1,0],True,3)):
            with self.subTest(labels=labels,k=k,total=total):
                with self.assertRaises(ValueError):
                    recall_at_k(labels,k,total)
if __name__=="__main__":
    unittest.main()
