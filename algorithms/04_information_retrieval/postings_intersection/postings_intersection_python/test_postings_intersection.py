import unittest
from postings_intersection import postings_intersection as intersect


class PostingIntersectionTests(unittest.TestCase):
    def test_matches(self):
        self.assertEqual(intersect([0,2,4,9],[1,2,4,8]), [2,4])
        self.assertEqual(intersect([0,2,4,9],[0,2,4,9]), [0,2,4,9])
        self.assertEqual(intersect([],[2,4]), [])
        self.assertEqual(intersect([] , []), [])

    def test_invalid(self):
        for a,b in (([1,1,2],[1]), ([5,2],[2]),
                    ([-1],[1]), ([2**31],[1]), ([True],[1]),
                    ([],[3,3]), ([],[4,2]), ([1.5],[1])):
            with self.subTest(a=a,b=b), self.assertRaises(ValueError):
                intersect(a,b)


if __name__ == "__main__":
    unittest.main()
