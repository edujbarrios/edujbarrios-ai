import math
import unittest
from weighted_knn_regression import weighted_knn_regression as predict


class WeightedKnnRegressionTests(unittest.TestCase):
    def test_examples(self):
        self.assertAlmostEqual(predict([[0],[4]],[0,20],[1],2),5)
        self.assertAlmostEqual(predict([[0],[2],[4]],[0,10,20],[1],2),5)
        self.assertAlmostEqual(predict([[0],[2],[4]],[0,10,20],[1],3),50/7)
        self.assertAlmostEqual(predict([[0],[0],[2]],[3,7,20],[0],3),5)
        self.assertAlmostEqual(predict([[0],[0],[2]],[3,7,20],[0],1),3)
        self.assertAlmostEqual(predict([[0],[0],[2]],[3,7,20],[0],2),5)
        self.assertAlmostEqual(predict([[-1],[1]],[2,8],[0],1),2)
        self.assertAlmostEqual(predict([[-1],[1]],[2,8],[0],2),5)
        self.assertAlmostEqual(predict([[0],[3],[5]],[-2,10,20],[1],2),2)
        self.assertAlmostEqual(predict([[4]],[12],[-3],1),12)
        self.assertAlmostEqual(predict([[0,0],[2,0],[0,2]],[2,4,6],[1,0],2),3)
        self.assertAlmostEqual(predict([(0,),(4,)],(0,20),(1,),2),5)
        x = [[0],[4]]
        y = [0,20]
        self.assertAlmostEqual(predict(x,y,[1],2),5)
        self.assertEqual(x,[[0],[4]])
        self.assertEqual(y,[0,20])

    def test_maximum(self):
        rows = [[0]*16 for _ in range(128)]
        self.assertAlmostEqual(predict(rows,[7]*128,[0]*16,128),7)
        self.assertAlmostEqual(predict([[0],[0],[1e-300]],[5,15,100],[0],3),10)

    def test_invalid(self):
        cases = [
            ([],[],[0],1),([[0]],[],[0],1),([[0]],[1],[],1),
            ([[0]],[1],[0],0),([[0]],[1],[0],2),
            ([[0]],[1],[0],True),([[0]],[1],[0],1.0),
            ([[0,1]],[1],[0],1),([[0]],[1],[0,1],1),
            ([[0]],["a"],[0],1),([[True]],[1],[0],1),
            ([[0]],[math.nan],[0],1),([[0]],[1],[math.inf],1),
            ([[1001]],[1],[0],1),([[0]],[1001],[0],1),
            (None,[1],[0],1),([[0]],None,[0],1),
            ([[0]],[1],None,1),([[0]]*129,[1]*129,[0],1),
            ([[0]*17],[1],[0]*17,1),([[0]],[1],[0],10**400),
        ]
        for args in cases:
            with self.subTest(args=str(args)[:80]), self.assertRaises(ValueError):
                predict(*args)


if __name__ == "__main__":
    unittest.main()
