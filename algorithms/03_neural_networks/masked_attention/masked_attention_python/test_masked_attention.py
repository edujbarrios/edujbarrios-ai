import unittest
from masked_attention import masked_attention as attention

class MaskedAttentionTests(unittest.TestCase):
    def test_subset_and_single_row(self):
        keys=[[1,0],[0,1],[1,1]]
        values=[[10,0],[0,20],[4,6]]
        result=attention([1,0],keys,values,[1,0,1])
        self.assertAlmostEqual(result[0],7.0)
        self.assertAlmostEqual(result[1],3.0)
        self.assertEqual(attention([1,0],keys,values,[0,1,0]),[0.0,20.0])
    def test_excluded_overflow(self):
        self.assertEqual(attention([1e308],[[1e308],[1]],[[0],[7]],[0,1]),[7.0])
        with self.assertRaises(ValueError):
            attention([1e308],[[1e308],[1]],[[0],[7]],[1,0])
    def test_invalid(self):
        cases=(([],[[1]],[[1]],[1]),([1],[],[],[]),
               ([1],[[1]],[[1]],[0]),([1],[[1]],[[1]],[2]),
               ([1],[[1]],[[1]],[True]),([1],[[1]],[[1]],[]),
               ([1],[[1],[2]],[[1],[2]],[1]),
               ([1],[[1]],[[float("nan")]],[1]),
               ([1],[[float("inf")]],[[1]],[0]))
        for q,k,v,m in cases:
            with self.subTest(query=q,mask=m),self.assertRaises(ValueError):
                attention(q,k,v,m)

if __name__=="__main__":
    unittest.main()
