import unittest
from postings_list import postings_list as postings


class PostingsTests(unittest.TestCase):
    def test_examples(self):
        corpus = [[1,2,1], [], [2,1], [3]]
        self.assertEqual(postings(corpus,1), [(0,2),(2,1)])
        self.assertEqual(postings(corpus,4), [])
        self.assertEqual(postings([], -4), [])
        self.assertEqual(postings([[-5,-5]],-5), [(0,2)])

    def test_invalid(self):
        for corpus, target in (([[True]],1), ([[2**31]],1),
                               ([[float("nan")]],1), ([1],1),
                               ([[1]],"1"), ([[1]],True),
                               ([[1]],-2**31-1)):
            with self.subTest(corpus=corpus,target=target), self.assertRaises(ValueError):
                postings(corpus,target)


if __name__ == "__main__":
    unittest.main()
