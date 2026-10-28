import unittest
from depthwise_conv2d import depthwise_conv2d as conv


class DepthwiseTests(unittest.TestCase):
    def test_reference(self):
        data=[1,2,3,4,5,6,7,8]
        kernels=[0,0,0,0,1,0,0,0,0]+[0,0,0,0,-1,0,0,0,0]
        self.assertEqual(conv(data,2,2,2,kernels,3,3),[1,2,3,4,-5,-6,-7,-8])
        self.assertEqual(conv(data[:4],1,2,2,[1]*9,3,3),[10]*4)
        self.assertEqual(conv([1],1,1,1,[1]*9,3,3),[1])
        self.assertEqual(conv([2,3],1,1,2,[2],1,1),[4,6])

    def test_invalid(self):
        x=[1,2,3,4]; k=[1]*9
        cases=[(x,0,2,2,k,3,3),(x,9,2,2,k,3,3),
               (x,1,0,2,k,3,3),(x,1,2,17,k,3,3),
               (x,1,2,2,k,2,3),(x,1,2,2,k,3,0),
               (x,True,2,2,k,3,3),(x[:-1],1,2,2,k,3,3),
               (x,1,2,2,k[:-1],3,3),
               ([1,float('nan'),3,4],1,2,2,k,3,3),
               (x,1,2,2,[1001],1,1),
               (x,1,2,2,[True],1,1)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    conv(*args)


if __name__ == "__main__":
    unittest.main()
