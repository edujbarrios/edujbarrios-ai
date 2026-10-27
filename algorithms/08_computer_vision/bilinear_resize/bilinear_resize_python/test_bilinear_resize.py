import unittest
from bilinear_resize import bilinear_resize as resize


class BilinearResizeTests(unittest.TestCase):
    def test_expected_interpolation(self):
        self.assertEqual(resize([0,10,20,30],2,2,3,3),
                         [0,5,10,10,15,20,20,25,30])
        self.assertEqual(resize([0,10,20,30],2,2,2,2),[0,10,20,30])
        self.assertEqual(resize([7],1,1,3,2),[7]*6)
        self.assertEqual(resize([0,10],1,2,1,3),[0,5,10])
        self.assertEqual(resize([0,10],2,1,3,1),[0,5,10])

    def test_invalid(self):
        image=[0,10,20,30]
        cases=[
            (image,0,2,3,3),(image,2,2,0,3),(image,2,2,65,3),
            (image,65,2,3,3),(image,2,2,3,True),
            ([0,10,20],2,2,3,3),([0,float("nan"),20,30],2,2,3,3),
            ([0,float("inf"),20,30],2,2,3,3),
            ([0,1000001,20,30],2,2,3,3),
            ([0,True,20,30],2,2,3,3)
        ]
        for args in cases:
            with self.subTest(args=str(args)[:70]):
                with self.assertRaises(ValueError):
                    resize(*args)


if __name__ == "__main__":
    unittest.main()
