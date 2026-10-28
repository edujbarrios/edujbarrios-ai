import unittest
from global_average_pool2d import global_average_pool2d as gap


class GAPTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(gap([1,2,3,4,5,6,7,8],2,2,2),[2.5,6.5])
        self.assertEqual(gap([-3,-1],1,1,2),[-2])
        self.assertEqual(gap([-3],1,1,1),[-3])
        self.assertEqual(gap([0]*18,2,3,3),[0,0])

    def test_invalid(self):
        a=[1,2,3,4]
        cases=[(a,0,2,2),(a,17,2,2),(a,1,0,2),
               (a,1,2,33),(a,True,2,2),(a,1,2,2.0),
               (a[:-1],1,2,2),([1,float('nan'),3,4],1,2,2),
               ([1,1001,3,4],1,2,2),([1,True,3,4],1,2,2),
               ('1234',1,2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    gap(*args)


if __name__ == "__main__":
    unittest.main()
