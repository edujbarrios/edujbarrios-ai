import unittest
from top_p_logit_filter import top_p_logit_filter as filt


class TopPTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(filt([0,0,0,0],0.6),[True,True,True,False])
        self.assertEqual(filt([0,0,0,0],0.1),[True,False,False,False])
        self.assertEqual(filt([0,0,0,0],1),[True]*4)
        self.assertEqual(filt([2,1,0],0.5),[True,False,False])
        self.assertEqual(filt([2,1,0],0.8),[True,True,False])
        self.assertEqual(filt([-1000,1000,0],0.9),[False,True,False])
        self.assertEqual(filt([5],0.3),[True])

    def test_invalid(self):
        cases=[([],0.5),([0]*4097,0.5),([0],0),([0],-0.1),
               ([0],1.1),([0],float('nan')),([0],True),
               ([float('nan')],0.5),([float('inf')],0.5),
               ([1001],0.5),([True],0.5),('abc',0.5)]
        for args in cases:
            with self.subTest(args=str(args)[:60]):
                with self.assertRaises(ValueError):
                    filt(*args)


if __name__ == "__main__":
    unittest.main()
