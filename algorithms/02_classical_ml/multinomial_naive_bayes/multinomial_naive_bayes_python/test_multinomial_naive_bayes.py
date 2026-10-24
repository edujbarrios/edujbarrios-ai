import math
import unittest
from multinomial_naive_bayes import multinomial_naive_bayes as train
from multinomial_naive_bayes import multinomial_naive_bayes_predict as predict

class MultinomialNaiveBayesTests(unittest.TestCase):
    def test_fit_and_predict(self):
        p,w=train([[3,0],[2,0],[0,3],[0,2]],[0,0,1,1])
        self.assertAlmostEqual(p[0],math.log(.5))
        self.assertAlmostEqual(p[1],math.log(.5))
        self.assertAlmostEqual(w[0][0],math.log(6/7))
        self.assertAlmostEqual(w[0][1],math.log(1/7))
        self.assertAlmostEqual(w[1][0],math.log(1/7))
        self.assertAlmostEqual(w[1][1],math.log(6/7))
        self.assertEqual(predict(p,w,[2,0]),0)
        self.assertEqual(predict(p,w,[0,2]),1)
        self.assertEqual(predict(p,w,[0,0]),0)
    def test_invalid(self):
        for x,y in (([],[]),([[0,1]],[0]),([[1,0],[0,1]],[0,0]),
                    ([[1,0],[0,1]],[0,2]),([[1,0],[0]],[0,1]),
                    ([[True,0],[0,1]],[0,1]),([[1000001,0],[0,1]],[0,1])):
            with self.subTest(x=x,y=y),self.assertRaises(ValueError):
                train(x,y)
        p,w=train([[1,0],[0,1]],[0,1])
        for priors,weights,q in ((p,w,[1]),(p,w,[1000001,0]),
                                 ([float("nan"),0],w,[1,0]),(p,[[1],[1]],[0,0])):
            with self.assertRaises(ValueError):
                predict(priors,weights,q)

if __name__=="__main__":unittest.main()
