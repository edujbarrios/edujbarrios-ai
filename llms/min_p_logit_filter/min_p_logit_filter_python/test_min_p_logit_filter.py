import unittest
from min_p_logit_filter import min_p_logit_filter as filt


class MinPTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(filt([0,-1,-2],.3),[True,True,False])
        self.assertEqual(filt([5,5,0],1),[True,True,False])
        self.assertEqual(filt([-1000,1000],0),[True,True])
        self.assertEqual(filt([-1000,1000],.5),[False,True])
        self.assertEqual(filt([-5],1),[True])

    def test_invalid(self):
        cases=[([],0),([0]*4097,.5),([0],-0.1),([0],1.1),
               ([0],float('nan')),([0],True),([True],.5),
               ([float('nan')],.5),([float('inf')],.5),
               ([1001],.5),('abc',.5)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    filt(*args)


if __name__ == "__main__":
    unittest.main()
