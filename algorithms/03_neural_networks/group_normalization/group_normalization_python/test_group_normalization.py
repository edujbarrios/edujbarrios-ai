import math
import unittest
from group_normalization import group_normalization as norm


class GroupNormTests(unittest.TestCase):
    def test_examples(self):
        x = [1,2,3,4,5,6,7,8]
        result = norm(x,4,2,2,[1]*4,[0]*4,0.25)
        expected = [v/math.sqrt(1.5) for v in [-1.5,-0.5,0.5,1.5]*2]
        for a,b in zip(result,expected):
            self.assertAlmostEqual(a,b,places=12)
        self.assertEqual(x,[1,2,3,4,5,6,7,8])
        self.assertEqual(norm([10,20],2,1,2,[3,4],[5,6],1e-5),[5,6])
        self.assertEqual(norm([7,7,7,7],2,2,2,[3,4],[5,6],1e-5),
                         [5,5,6,6])
        self.assertEqual(norm([9],1,1,1,[2],[3],1e-5),[3])
        self.assertEqual(norm([1000],1,1,1,[-1000],[-999],1e-5),[-999])
        result = norm([1,3,5,7],2,2,1,[2,0.5],[1,-1],1.0)
        expected = [1-6/math.sqrt(6),1-2/math.sqrt(6),
                    -1+0.5/math.sqrt(6),-1+1.5/math.sqrt(6)]
        for a,b in zip(result,expected):
            self.assertAlmostEqual(a,b)
        self.assertEqual(norm((0,0),2,1,1,(1,2),(3,4),1e-5),[3,4])
        separated = norm([1,3,4,6],2,2,2,[1,1],[0,0],1.0)
        for actual, expected in zip(separated,[-1/math.sqrt(2),1/math.sqrt(2)]*2):
            self.assertAlmostEqual(actual,expected)

    def test_bounds(self):
        x = [0]*2048
        self.assertEqual(norm(x,32,64,8,[1]*32,[0]*32,1e-5),[0]*2048)
        self.assertEqual(norm([0]*2048,32,64,32,[1]*32,[2]*32,1e-5),[2]*2048)
        self.assertEqual(norm([0]*2048,32,64,1,[1]*32,[3]*32,1e-5),[3]*2048)

    def test_invalid(self):
        valid = ([1],1,1,1,[1],[0],1e-5)
        cases = [
            ([1],0,1,1,[1],[0],1e-5),
            ([1],33,1,1,[1],[0],1e-5),
            ([1],1,0,1,[1],[0],1e-5),
            ([1],1,65,1,[1],[0],1e-5),
            ([1],1,1,0,[1],[0],1e-5),
            ([1,2,3],3,1,2,[1]*3,[0]*3,1e-5),
            ([1],True,1,1,[1],[0],1e-5),
            ([1],1,1,1.0,[1],[0],1e-5),
            ([],1,1,1,[1],[0],1e-5),
            ([1],1,1,1,[],[0],1e-5),
            ([1],1,1,1,[1],[],1e-5),
            ([True],1,1,1,[1],[0],1e-5),
            ([1],1,1,1,[math.inf],[0],1e-5),
            ([1],1,1,1,[1],[math.nan],1e-5),
            ([1001],1,1,1,[1],[0],1e-5),
            ([1],1,1,1,[1],[0],0),
            ([1],1,1,1,[1],[0],-1),
            ([1],1,1,1,[1],[0],2),
            ([1],1,1,1,[1],[0],math.inf),
            ([1],1,1,1,[1],[0],True),
            (None,1,1,1,[1],[0],1e-5),
        ]
        self.assertEqual(norm(*valid),[0])
        for args in cases:
            with self.subTest(args=str(args)[:60]),self.assertRaises(ValueError):
                norm(*args)


if __name__ == "__main__":
    unittest.main()
