import unittest
from farthest_first_traversal import farthest_first_traversal as select

class FarthestFirstTests(unittest.TestCase):
    def test_centers_and_ties(self):
        points=[[0,0],[1,0],[9,0],[10,0]]
        self.assertEqual(select(points,3),[0,3,1])
        self.assertEqual(select(points,4),[0,3,1,2])
        self.assertEqual(select([[0,0],[0,0],[2,0],[2,0]],3),[0,2,1])
        self.assertEqual(select([[0]],1),[0])
        self.assertEqual(select([[0,0],[0,0]],2),[0,1])
    def test_invalid(self):
        for points,k in (([],1),([[]],1),([[0]],0),([[0]],2),
                         ([[1],[1,2]],1),([[0,0],[float("nan"),0]],1),
                         ([[0,0],[True,0]],1),([[1.7e308],[-1.7e308]],2)):
            with self.subTest(points=points,k=k),self.assertRaises(ValueError):
                select(points,k)

if __name__=="__main__":unittest.main()
