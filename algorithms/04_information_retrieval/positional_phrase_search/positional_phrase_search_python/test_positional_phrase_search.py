import unittest
from positional_phrase_search import positional_phrase_search as phrase

class PhraseSearchTests(unittest.TestCase):
    def test_adjacency_and_document_order(self):
        a=[(1,0),(1,5),(2,1),(4,2)]
        b=[(1,1),(1,8),(2,3),(4,3)]
        self.assertEqual(phrase(a,b),[1,4])
        self.assertEqual(phrase([],b),[])
        self.assertEqual(phrase(a,[]),[])
        self.assertEqual(phrase([(5,2)],[(5,1)]),[])
        self.assertEqual(phrase([(1,0),(1,4)],[(1,1),(1,5)]),[1])
    def test_invalid(self):
        valid=[(1,0)]
        for bad in ([(1,5),(1,0)],[(1,0),(1,0)],[(True,0)],
                    [(1,-1)],[(1,1_000_000_001)],[(1,0.5)],[(1,)]):
            with self.subTest(bad=bad),self.assertRaises(ValueError):
                phrase(bad,valid)
        with self.assertRaises(ValueError):
            phrase(valid,[(1,1),(1,0)])

if __name__=="__main__":unittest.main()
