import unittest
from max_pool2d import max_pool2d as pool


class PoolTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(pool(list(range(1,17)),4,4),[6,8,14,16])
        self.assertEqual(pool(list(range(1,10)),3,3),[5])
        self.assertEqual(pool([-4,-2,-1,-3],2,2),[-1])
        self.assertEqual(pool([2,1,3,4,5,7],2,3),[4])

    def test_invalid(self):
        good=[1,2,3,4]
        cases=[(good,1,2),(good,2,1),(good,33,2),
               (good,True,2),(good,2,2.0),(good[:-1],2,2),
               (good[:1]+[float('nan')]+good[2:],2,2),
               (good[:1]+[1001]+good[2:],2,2),
               (good[:1]+[True]+good[2:],2,2),
               ('1234',2,2)]
        for args in cases:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    pool(*args)


if __name__=="__main__":
    unittest.main()
