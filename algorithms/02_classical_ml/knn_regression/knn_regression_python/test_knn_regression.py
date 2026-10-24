import math
import unittest
from knn_regression import knn_regression as predict

class KNNRegressionTests(unittest.TestCase):
    def test_neighbors(self):
        training=[[0,0],[2,0],[0,2],[5,5]]
        targets=[2,4,6,10]
        for k,expected in ((1,2),(2,3),(3,4),(4,5.5)):
            with self.subTest(k=k):
                self.assertAlmostEqual(predict(training,targets,[0,0],k),expected)
        self.assertEqual(predict([[0,0],[0,0]],[-5,7],[0,0],2),1)
        self.assertEqual(predict([[0,0]],[2],[0,0],1),2)
        self.assertAlmostEqual(predict([[0,0],[0,0]],[1e308,1e308],[0,0],2)/1e308,1)
    def test_invalid(self):
        pts=[[0,0],[2,0],[0,2],[5,5]]
        y=[2,4,6,10]
        cases=(([],[],[0,0],1),(pts,y,[0,0],0),(pts,y,[0,0],5),
               (pts,y[:2],[0,0],1),(pts,y,[0],1),
               ([[0,0],[0]],y[:2],[0,0],1),
               ([[0,0],[math.nan,0]],y[:2],[0,0],1),
               ([[-1.7e308],[1.7e308]],[-5,7],[-1.7e308],1),
               (pts,y,[0,0],True),(pts,[2,4,math.nan,10],[0,0],1))
        for item in cases:
            with self.subTest(args=item),self.assertRaises(ValueError):
                predict(*item)

if __name__=="__main__":unittest.main()
