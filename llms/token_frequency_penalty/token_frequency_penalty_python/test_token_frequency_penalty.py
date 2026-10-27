import unittest
from token_frequency_penalty import token_frequency_penalty as penalize


class FrequencyPenaltyTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(penalize([1,2,3],[0,2,2],.5),[.5,2,2])
        self.assertEqual(penalize([-1,0,1],[0,0],1),[-3,0,1])
        self.assertEqual(penalize([1,2,3],[],1),[1,2,3])
        self.assertEqual(penalize([1,2,3],[0,2,2],0),[1,2,3])

    def test_invalid(self):
        a=[1,2,3]
        cases=[([],[],1),(a,[3],1),(a,[-1],1),
               (a,[True],1),(a,[1.0],1),(a,[0]*4097,1),
               (a,[0],-1),(a,[0],6),(a,[0],True),
               (a,[0],float('nan')),(a,[0],float('inf')),
               ([1,float('nan'),3],[],1),([1001],[],1),
               ([True,2],[],1),(a,'0',1)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    penalize(*args)


if __name__ == "__main__":
    unittest.main()
