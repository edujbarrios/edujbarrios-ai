import unittest
from valid_conv2d import valid_conv2d as conv


class ConvTests(unittest.TestCase):
    def test_reference(self):
        im=[1,2,3,4,5,6,7,8,9]
        self.assertEqual(conv(im,3,3,[1,0,0,-1],2,2),[-4]*4)
        self.assertEqual(conv(im,3,3,[-2],1,1),[-2*x for x in im])
        self.assertEqual(conv([1,2,3,4,5,6],2,3,[1,0,0,-1],2,2),[-4,-4])
        self.assertEqual(conv([5],1,1,[3],1,1),[15])

    def test_invalid(self):
        im=[1,2,3,4,5,6,7,8,9]
        k=[1,0,0,-1]
        bad=[(im,0,3,k,2,2),(im,3,0,k,2,2),
             (im,33,3,k,2,2),(im,3,3,k,4,2),
             (im,3,3,k,0,2),(im,True,3,k,2,2),
             (im[:-1],3,3,k,2,2),(im,3,3,k[:-1],2,2),
             (im,3,3,[1,float('nan'),0,-1],2,2),
             (im,3,3,[1,1001,0,-1],2,2),
             (im,3,3,[1,True,0,-1],2,2)]
        for args in bad:
            with self.subTest(args=str(args)[:65]):
                with self.assertRaises(ValueError):
                    conv(*args)


if __name__ == "__main__":
    unittest.main()
