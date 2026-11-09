import math
import unittest
from average_pool2d import average_pool2d as pool


class AveragePoolTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(pool(list(range(1,17)),4,4),[3.5,5.5,11.5,13.5])
        self.assertEqual(pool(list(range(1,16)),3,5),[4.0,6.0])
        self.assertEqual(pool([1,2,3,4],2,2),[2.5])
        self.assertEqual(pool([-4,-2,-6,-8],2,2),[-5])
        self.assertEqual(pool([0]*4,2,2),[0])
        self.assertEqual(pool([1000]*4,2,2),[1000])
        self.assertEqual(pool([-1000]*4,2,2),[-1000])
        self.assertEqual(pool((1,3,5,7),2,2),[4.0])
        self.assertEqual(pool([1,2,3,4,5,6],2,3),[3])
        self.assertEqual(pool([1,2,3,4,5,6],3,2),[2.5])
        self.assertEqual(pool([1,1,1000,1,1,1000,1000,1000,1000],3,3),[1])  # dropped borders
        image = [1,2,3,4]
        self.assertEqual(pool(image,2,2),[2.5])
        self.assertEqual(image,[1,2,3,4])

    def test_maximum(self):
        self.assertEqual(pool([1]*1024,32,32),[1]*256)
        checker = [(-1 if (r+c)%2 else 1) for r in range(32) for c in range(32)]
        self.assertEqual(pool(checker,32,32),[0]*256)

    def test_invalid(self):
        cases = [
            ([1],1,2),([1],2,1),([1],33,2),([1],2,33),
            ([1],True,2),([1],2,2.0),([1],2,False),
            ([],2,2),([1]*3,2,2),([1]*5,2,2),
            ([1,2,3,math.nan],2,2),([1,2,3,math.inf],2,2),
            ([1,2,3,1001],2,2),([1,2,3,-1001],2,2),
            ([True,2,3,4],2,2),(["1",2,3,4],2,2),
            (None,2,2),("1234",2,2),([10**400,0,0,0],2,2),
        ]
        for args in cases:
            with self.subTest(args=str(args)[:50]), self.assertRaises(ValueError):
                pool(*args)


if __name__ == "__main__":
    unittest.main()
