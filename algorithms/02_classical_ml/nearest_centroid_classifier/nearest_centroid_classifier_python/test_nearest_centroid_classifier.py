import math
import unittest
from nearest_centroid_classifier import nearest_centroid_classifier as fit
from nearest_centroid_classifier import nearest_centroid_classifier_predict as predict

class NearestCentroidTests(unittest.TestCase):
    def test_train_and_predict(self):
        centers=fit([[0,0],[2,0],[8,0],[10,0]],[0,0,1,1])
        self.assertEqual(centers,[[1,0],[9,0]])
        self.assertEqual(predict(centers,[2,0]),0)
        self.assertEqual(predict(centers,[8,0]),1)
        self.assertEqual(predict(centers,[5,0]),0)
    def test_invalid(self):
        tests=(([],[]),([[0,0]],[0]),([[0],[1]],[0,0]),
               ([[0],[1]],[0,2]),([[0],[1]],[0,1,1]),
               ([[0],[True]],[0,1]),([[0],[math.nan]],[0,1]))
        for rows,labels in tests:
            with self.subTest(rows=rows,labels=labels),self.assertRaises(ValueError):
                fit(rows,labels)
        centers=fit([[0],[10]],[0,1])
        for value in ([math.nan],[1,2],[1e308,-1e308]):
            with self.subTest(q=value),self.assertRaises(ValueError):
                predict(centers,value)

if __name__=="__main__":unittest.main()
