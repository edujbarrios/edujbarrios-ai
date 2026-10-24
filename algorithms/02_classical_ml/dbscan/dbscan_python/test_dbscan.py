import unittest
from dbscan import dbscan

class DBSCANTests(unittest.TestCase):
    def test_clusters_noise_and_border(self):
        points=[[0,0],[.1,0],[0,.1],[5,5],[5.1,5],[9,9]]
        self.assertEqual(dbscan(points,.2,2),[0,0,0,1,1,-1])
        self.assertEqual(dbscan([[0],[1],[2],[3]],1.1,3),[0,0,0,0])
        self.assertEqual(dbscan([[1],[1],[1]],.1,3),[0,0,0])
        self.assertEqual(dbscan([[0],[10],[20]],1,2),[-1,-1,-1])
        self.assertEqual(dbscan([[0]],1,1),[0])
    def test_invalid(self):
        cases=(([],1,1),([[0]],0,1),([[0]],1,0),([[0]],1,2),
               ([[0],[0,1]],1,1),([[0],[float("nan")]],1,1),
               ([[0],[True]],1,1),([[-1.7e308],[1.7e308]],1,1))
        for x,eps,m in cases:
            with self.subTest(x=x,eps=eps,m=m),self.assertRaises(ValueError):
                dbscan(x,eps,m)

if __name__=="__main__":unittest.main()
