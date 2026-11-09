import math
import unittest
from same_conv2d import same_conv2d as conv


class SameConv2dTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(conv([1,2,3,4,5,6,7,8,9],3,3,[1]*9,3,3),
                         [12,21,16,27,45,33,24,39,28])
        self.assertEqual(conv([1,2,3,4],2,2,[1]*9,3,3),[10]*4)
        self.assertEqual(conv([5],1,1,[1]*9,3,3),[5])
        self.assertEqual(conv([1,2,3],1,3,[1,2,3],1,3),[8,14,8])
        self.assertEqual(conv([1,2,3],3,1,[1,2,3],3,1),[8,14,8])
        self.assertEqual(conv([-1,2,3,-4],2,2,[-2],1,1),[2,-4,-6,8])
        self.assertEqual(conv([1,2,3,4],2,2,[0],1,1),[0]*4)
        self.assertEqual(conv([10],1,1,[0]*25,5,5),[0])
        self.assertEqual(conv((3,),1,1,(2,),1,1),[6])
        x = [1,2,3,4]
        k = [1,2,3]
        conv(x,2,2,k,1,3)
        self.assertEqual(x,[1,2,3,4])
        self.assertEqual(k,[1,2,3])

    def test_maximum(self):
        self.assertEqual(conv([1]*1024,32,32,[1],1,1),[1]*1024)
        self.assertEqual(conv([0]*1024,32,32,[1]*25,5,5),[0]*1024)
        self.assertEqual(conv([1000],1,1,[1000],1,1),[1000000])

    def test_invalid(self):
        cases = [
            ([1],0,1,[1],1,1),([1],1,0,[1],1,1),
            ([1],33,1,[1],1,1),([1],1,33,[1],1,1),
            ([1],True,1,[1],1,1),([1],1,1,[1],2,1),
            ([1],1,1,[1],1,0),([1],1,1,[1],6,1),
            ([],1,1,[1],1,1),([1],1,1,[],1,1),
            ([math.nan],1,1,[1],1,1),([1],1,1,[math.inf],1,1),
            ([1001],1,1,[1],1,1),([1],1,1,[-1001],1,1),
            ([True],1,1,[1],1,1),([1],1,1,[True],1,1),
            (None,1,1,[1],1,1),([1],1,1,None,1,1),
        ]
        for args in cases:
            with self.subTest(args=str(args)[:60]), self.assertRaises(ValueError):
                conv(*args)


if __name__ == "__main__":
    unittest.main()
