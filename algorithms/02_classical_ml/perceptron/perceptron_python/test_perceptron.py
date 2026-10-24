import unittest
from perceptron import perceptron, perceptron_predict

class PerceptronTests(unittest.TestCase):
    def test_training(self):
        x=[[-2,-1],[-1,-1],[1,1],[2,1]]
        w,b=perceptron(x,[0,0,1,1],1.0,1)
        self.assertEqual((w,b),([1.0,1.0],1.0))
        self.assertEqual(perceptron_predict(w,[-2,-1],b),0)
        self.assertEqual(perceptron_predict(w,[2,1],b),1)
        self.assertEqual(perceptron([[0,0]],[1],1.0,1),([0.0,0.0],1.0))
        self.assertEqual(perceptron_predict([0,0],[0,0],0.0),0)
    def test_invalid(self):
        tests=(([],[],1,1),([[1]],[],1,1),([[]],[0],1,1),
               ([[1],[1,2]],[0,1],1,1),([[0]],[2],1,1),
               ([[True]],[0],1,1),([[float("nan")]],[0],1,1),
               ([[0]],[1],0,1),([[0]],[1],1,0),
               ([[1e308]],[1],1e308,2))
        for x,y,lr,epochs in tests:
            with self.subTest(x=x,y=y,lr=lr,epochs=epochs):
                with self.assertRaises(ValueError):
                    perceptron(x,y,lr,epochs)
        with self.assertRaises(ValueError):
            perceptron_predict([float("inf")],[1],0)

if __name__=="__main__":
    unittest.main()
