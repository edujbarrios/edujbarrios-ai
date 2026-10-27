import unittest
from binary_erosion import binary_erosion as erode


class ErosionTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(erode([1]*9,3,3),[0,0,0,0,1,0,0,0,0])
        self.assertEqual(erode([0]*9,3,3),[0]*9)
        self.assertEqual(erode([1,1,1,1,0,1,1,1,1],3,3),[0]*9)
        self.assertEqual(erode([1,1,1],1,3),[0]*3)
        self.assertEqual(erode([1],1,1),[0])
        self.assertEqual(erode([1]*25,5,5),[
            0,0,0,0,0,
            0,1,1,1,0,
            0,1,1,1,0,
            0,1,1,1,0,
            0,0,0,0,0])

    def test_invalid(self):
        a=[1]*9
        cases=[(a,0,3),(a,3,0),(a,65,3),(a,3,65),(a,True,3),
               (a,3,3.0),(a[:-1],3,3),(a[:4]+[2]+a[5:],3,3),
               (a[:4]+[True]+a[5:],3,3),
               (a[:4]+[1.0]+a[5:],3,3),('111111111',3,3)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    erode(*args)


if __name__ == "__main__":
    unittest.main()
