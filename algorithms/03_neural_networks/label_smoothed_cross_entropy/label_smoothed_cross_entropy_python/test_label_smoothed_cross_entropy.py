import math
import unittest
from label_smoothed_cross_entropy import label_smoothed_cross_entropy as loss_fn

class LabelSmoothingTests(unittest.TestCase):
    def test_values(self):
        z=[0,math.log(3)]
        loss,g=loss_fn(z,1,.2)
        self.assertAlmostEqual(loss,-.1*math.log(.25)-.9*math.log(.75))
        self.assertAlmostEqual(g[0],.15)
        self.assertAlmostEqual(g[1],-.15)
        loss,g=loss_fn(z,1,0)
        self.assertAlmostEqual(loss,-math.log(.75))
        self.assertAlmostEqual(g[0],.25)
        loss,g=loss_fn(z,1,1)
        self.assertAlmostEqual(g[0],-.25)
        self.assertAlmostEqual(g[1],.25)
        self.assertAlmostEqual(loss_fn([0,0],0,.5)[0],math.log(2))
        self.assertEqual(loss_fn([12],0,.6),(0.0,[0.0]))
        self.assertTrue(math.isfinite(loss_fn([1000,1001],1,.2)[0]))
    def test_invalid(self):
        for a,y,e in (([],0,.2),([0,1],2,.2),([0,1],True,.2),
                      ([0,1],1,-.1),([0,1],1,1.1),
                      ([0,math.nan],1,.2),([-1e308,1e308],1,.2),
                      ([True,0],1,.2),([0,1],1,True)):
            with self.subTest(logits=a,target=y,epsilon=e),self.assertRaises(ValueError):
                loss_fn(a,y,e)

if __name__=="__main__":unittest.main()
