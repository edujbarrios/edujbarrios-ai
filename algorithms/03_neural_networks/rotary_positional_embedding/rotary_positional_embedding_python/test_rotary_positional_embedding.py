import math
import unittest
from rotary_positional_embedding import rotary_positional_embedding as rope

class RotaryTests(unittest.TestCase):
    def test_examples(self):
        q,k=rope([1,0],[0,1],1,0)
        self.assertAlmostEqual(q[0],math.cos(1))
        self.assertAlmostEqual(q[1],math.sin(1))
        self.assertAlmostEqual(k[0],0)
        self.assertAlmostEqual(k[1],1)
        q,k=rope([3,4],[5,6],0,0)
        self.assertEqual(q,[3,4])
        self.assertEqual(k,[5,6])
    def test_frequencies_and_shared_position(self):
        q,k=rope([1,0,1,0],[1,0,1,0],1,1)
        self.assertAlmostEqual(q[2],math.cos(.01))
        self.assertAlmostEqual(q[3],math.sin(.01))
        self.assertEqual(q,k)
        a,b=rope([1,2],[3,4],777,777)
        self.assertAlmostEqual(sum(x*y for x,y in zip(a,b)),11,delta=1e-12)
    def test_invalid(self):
        for q,k,qp,kp in (([1],[0],1,0),([1,0,2],[0,1,2],1,0),
                          ([1,0],[0],1,0),([1,0],[0,1],-1,0),
                          ([1,0],[0,1],1,1000001),
                          ([1,0],[0,1],True,0),
                          ([float("nan"),0],[0,1],1,0),
                          ([1.7e308,1.7e308],[0,1],1,0)):
            with self.subTest(q=q,k=k,qp=qp,kp=kp),self.assertRaises(ValueError):
                rope(q,k,qp,kp)

if __name__=="__main__":
    unittest.main()
