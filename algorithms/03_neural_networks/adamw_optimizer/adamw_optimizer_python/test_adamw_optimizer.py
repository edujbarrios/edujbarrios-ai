import math
import unittest
from adamw_optimizer import adamw_optimizer as step

class AdamWTests(unittest.TestCase):
    def test_first_and_second_update(self):
        theta,m,v=step([2,-3],[1,-2],[0,0],[0,0],1,.1)
        self.assertAlmostEqual(m[0],.1)
        self.assertAlmostEqual(m[1],-.2)
        self.assertAlmostEqual(v[0],.001)
        self.assertAlmostEqual(v[1],.004)
        self.assertAlmostEqual(theta[0],2-.1*(1/(1+1e-8)+.02))
        self.assertAlmostEqual(theta[1],-3-.1*(-2/(2+1e-8)-.03))
        new_theta,new_m,new_v=step(theta,[0,0],m,v,2,.1)
        self.assertAlmostEqual(new_m[0],.09)
        self.assertAlmostEqual(new_v[0],.000999)
        self.assertLess(new_theta[0],theta[0])
        self.assertGreater(new_theta[1],theta[1])
        self.assertAlmostEqual(step([1],[0],[0],[0],1,.1)[0][0],.999)
    def test_invalid(self):
        cases=(([],[],[],[],1,.1),([1],[],[0],[0],1,.1),
               ([1],[1],[0],[-1],1,.1),([1],[1],[0],[0],0,.1),
               ([1],[1],[0],[0],1,0),([1],[math.nan],[0],[0],1,.1),
               ([1],[1e308],[0],[0],1,.1),([True],[1],[0],[0],1,.1))
        for item in cases:
            with self.subTest(item=item),self.assertRaises(ValueError):
                step(*item)

if __name__=="__main__":unittest.main()
