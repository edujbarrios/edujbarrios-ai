import math
import unittest
from token_presence_penalty import token_presence_penalty as adjust

class PresencePenaltyTests(unittest.TestCase):
    def test_duplicate_tokens(self):
        self.assertEqual(adjust([1,2,3],[0,2,2],0.5),[0.5,2.0,2.5])
        self.assertEqual(adjust([-1,0,1],[0,0],1),[-2.0,0.0,1.0])

    def test_noop(self):
        self.assertEqual(adjust([1,2,3],[],5),[1,2,3])
        self.assertEqual(adjust([1,2,3],[0,2,2],0),[1,2,3])

    def test_invalid(self):
        cases = [([],[],1),([1],[0],-1),([1],[0],math.nan),
                 ([1001],[],1),([math.inf],[],1),([True],[],1),
                 ([1],[1],1),([1],[-1],1),([1],[True],1),
                 ([1],[0],True),([1],[],10**400)]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                adjust(*args)

if __name__=="__main__":
    unittest.main()
