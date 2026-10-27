import unittest
from binary_dilation import binary_dilation as dilate


class DilationTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(dilate([0,0,0,0,1,0,0,0,0],3,3),[1]*9)
        self.assertEqual(dilate([1,0,0,0,0,0,0,0,0],3,3),
                         [1,1,0,1,1,0,0,0,0])
        self.assertEqual(dilate([0,1,0],1,3),[1,1,1])
        self.assertEqual(dilate([0],1,1),[0])
        self.assertEqual(dilate([1],1,1),[1])

    def test_invalid(self):
        mask=[0,0,0,0,1,0,0,0,0]
        cases=[(mask,0,3),(mask,3,0),(mask,65,3),
               (mask,3,65),(mask,True,3),
               (mask[:-1],3,3),(mask[:4]+[2]+mask[5:],3,3),
               (mask[:4]+[True]+mask[5:],3,3),
               (mask[:4]+[1.0]+mask[5:],3,3),
               ('000010000',3,3)]
        for args in cases:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    dilate(*args)


if __name__ == "__main__":
    unittest.main()
