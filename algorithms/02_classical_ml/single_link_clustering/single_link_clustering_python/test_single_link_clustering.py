import unittest
from single_link_clustering import single_link_clustering as cluster


class SingleLinkClusteringTests(unittest.TestCase):
    def test_reference(self):
        p = [0,0, 0,1, 10,10, 10,11]
        self.assertEqual(cluster(p,4,2), [0,0,1,1])
        self.assertEqual(cluster(p,4,3), [0,0,1,2])
        self.assertEqual(cluster(p,4,1), [0,0,0,0])
        self.assertEqual(cluster(p,4,4), [0,1,2,3])
        self.assertEqual(cluster([0,0, 0,2, 2,0, 2,2],4,2), [0,0,0,1])
        self.assertEqual(cluster([0,0, 0,0, 10,10],3,2), [0,0,1])
        self.assertEqual(cluster([-1000,-1000, 1000,1000],2,1), [0,0])
        self.assertEqual(cluster([0,0],1,1), [0])
        big = [x for i in range(32) for x in (i,0)]
        self.assertEqual(cluster(big,32,1), [0]*32)
        self.assertEqual(cluster(big,32,32), list(range(32)))
        self.assertEqual(p, [0,0, 0,1, 10,10, 10,11])

    def test_invalid(self):
        p = [0,0, 0,1, 10,10, 10,11]
        cases = [
            (p,0,1), (p,33,2), (p,4,0), (p,4,5),
            (p,True,2), (p,4,False), (p,4.0,2), (p,4,2.0),
            (p[:-1],4,2), (None,4,2), ([0,0, 1001,0],2,1),
            ([0,0, -1001,0],2,1), ([0,0, 1.5,0],2,1),
            ([0,0, True,0],2,1), ([0,0, float("nan"),0],2,1),
        ]
        for args in cases:
            with self.subTest(args=args), self.assertRaises(ValueError):
                cluster(*args)


if __name__ == "__main__":
    unittest.main()
