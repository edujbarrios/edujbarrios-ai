import unittest
from mini_batch_gradient_descent import mini_batch_gradient_descent as train

class MiniBatchTests(unittest.TestCase):
    def test_ordered_one_epoch(self):
        w,b=train([0,1],[1,3],0.1,1,1)
        self.assertAlmostEqual(w,0.29)
        self.assertAlmostEqual(b,0.39)
    def test_fit_and_last_batch(self):
        w,b=train([0,1,2,3],[1,3,5,7],0.05,2000,3)
        self.assertAlmostEqual(w,2.0,delta=0.01)
        self.assertAlmostEqual(b,1.0,delta=0.01)
    def test_invalid(self):
        for x,y,lr,epochs,batch in (([],[],0.1,1,1),
                                    ([1],[],0.1,1,1),
                                    ([1],[1],0,1,1),
                                    ([1],[1],0.1,0,1),
                                    ([1],[1],0.1,1,0),
                                    ([1],[1],0.1,1,2),
                                    ([float("nan")],[1],0.1,1,1),
                                    ([1e308,1e308],[1,3],1e308,2,2),
                                    ([True],[1],0.1,1,1)):
            with self.subTest(x=x,lr=lr,epochs=epochs,batch=batch),self.assertRaises(ValueError):
                train(x,y,lr,epochs,batch)

if __name__=="__main__": unittest.main()
