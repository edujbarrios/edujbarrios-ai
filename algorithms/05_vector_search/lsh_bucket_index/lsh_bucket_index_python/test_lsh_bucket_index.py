import unittest
from lsh_bucket_index import build_lsh_bucket_index

class BucketIndexTests(unittest.TestCase):
    def test_reusable_index(self):
        idx=build_lsh_bucket_index([3,7,3,0,7])
        self.assertEqual(idx.query(3),[0,2])
        self.assertEqual(idx.query(7),[1,4])
        self.assertEqual(idx.query(4),[])
        self.assertEqual(idx.query(3),[0,2])
        self.assertEqual(build_lsh_bucket_index([(1<<30)-1]).query((1<<30)-1),[0])
    def test_invalid(self):
        for signatures in ([],[3,1<<30],[3,-1],[0,True],[0,0.5]):
            with self.subTest(signatures=signatures),self.assertRaises(ValueError):
                build_lsh_bucket_index(signatures)
        idx=build_lsh_bucket_index([3])
        for q in (-1,1<<30,True,0.5):
            with self.subTest(q=q),self.assertRaises(ValueError):
                idx.query(q)

if __name__=="__main__":unittest.main()
