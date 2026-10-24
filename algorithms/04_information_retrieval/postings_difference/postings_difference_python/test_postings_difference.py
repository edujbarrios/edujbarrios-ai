import unittest
from postings_difference import postings_difference as difference

class PostingsDifferenceTests(unittest.TestCase):
    def test_results_and_empty(self):
        self.assertEqual(difference([0,2,4,7],[1,2,5,7]),[0,4])
        self.assertEqual(difference([0,2,4],[0,2,4]),[])
        self.assertEqual(difference([0,2,4],[]),[0,2,4])
        self.assertEqual(difference([],[0,2,4]),[])
        self.assertEqual(difference([],[]),[])
        self.assertEqual(difference([2**31-1],[]),[2**31-1])
    def test_invalid(self):
        for left,right in (([1,1],[]),([],[5,4]),
                           ([-1],[0]),([2**31],[]),
                           ([1.5],[0]),([True],[]),
                           ([],[0,0]),([0],[False])):
            with self.subTest(left=left,right=right),self.assertRaises(ValueError):
                difference(left,right)

if __name__=="__main__":
    unittest.main()
