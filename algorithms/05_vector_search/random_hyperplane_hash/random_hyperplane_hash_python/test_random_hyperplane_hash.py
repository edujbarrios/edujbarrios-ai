import unittest
from random_hyperplane_hash import random_hyperplane_hash as lsh

class HyperplaneHashTests(unittest.TestCase):
    def test_signatures(self):
        normals = [[1,0],[0,1],[-1,-1]]
        self.assertEqual(lsh([1,2], normals), 3)
        self.assertEqual(lsh([0,1], normals), 3)
        self.assertEqual(lsh([-1,-2], normals), 4)
        self.assertEqual(lsh([1], [[1] for _ in range(30)]), (1<<30)-1)
    def test_invalid(self):
        for x,normals in (([],[[1]]),([0,0],[[1,0]]),
                          ([1],[]),([1],[[0]]),([1],[[]]),
                          ([1],[[1] for _ in range(31)]),
                          ([1],[["a"]]),([True],[[1]]),
                          ([float("nan")],[[1]]),([1e308],[[1e308]])):
            with self.subTest(x=x,normals=normals), self.assertRaises(ValueError):
                lsh(x,normals)

if __name__ == "__main__":
    unittest.main()
