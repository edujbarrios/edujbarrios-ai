import unittest
from postings_union import postings_union as union

class PostingsUnionTests(unittest.TestCase):
    def test_overlap_and_empty(self):
        self.assertEqual(union([0,2,4],[1,2,5]),[0,1,2,4,5])
        self.assertEqual(union([],[0,2,4]),[0,2,4])
        self.assertEqual(union([0,2,4],[]),[0,2,4])
        self.assertEqual(union([],[]),[])
        self.assertEqual(union([2**31-1],[]),[2**31-1])
    def test_invalid(self):
        for left,right in (([1,1],[]),([],[5,4]),
                           ([-1],[1]),([2**31],[]),
                           ([1.0],[]),([True],[]),
                           ([],[False]),([1],[0,0])):
            with self.subTest(left=left,right=right),self.assertRaises(ValueError):
                union(left,right)

if __name__=="__main__":
    unittest.main()
