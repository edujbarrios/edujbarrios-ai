import unittest
from adam_optimizer import adam_optimizer as step

class AdamTests(unittest.TestCase):
    def test_first_and_second_step(self):
        p,m,v=step([2,-3],[1,-2],[0,0],[0,0],0.1,1)
        self.assertAlmostEqual(p[0],2-0.1/(1+1e-8))
        self.assertAlmostEqual(p[1],-3+0.2/(2+1e-8))
        self.assertAlmostEqual(m[0],0.1)
        self.assertAlmostEqual(m[1],-0.2)
        self.assertAlmostEqual(v[0],0.001)
        self.assertAlmostEqual(v[1],0.004)
        _,next_m,next_v=step(p,[0,0],m,v,0.1,2)
        self.assertLess(next_m[0],m[0])
        self.assertGreater(next_m[1],m[1])
        self.assertGreater(next_v[0],0)
    def test_invalid(self):
        cases=(([1],[],[0],[0],0.1,1),([],[],[],[],0.1,1),
               ([1],[1],[0],[-1],0.1,1),
               ([1],[float("nan")],[0],[0],0.1,1),
               ([1],[1],[0],[0],0.0,1),
               ([1],[1],[0],[0],0.1,0),
               ([1],[1e308],[0],[0],0.1,1),
               ([1],[1],[0],[0],0.1,True))
        for args in cases:
            with self.subTest(args=args),self.assertRaises(ValueError):
                step(*args)

if __name__=="__main__":unittest.main()
