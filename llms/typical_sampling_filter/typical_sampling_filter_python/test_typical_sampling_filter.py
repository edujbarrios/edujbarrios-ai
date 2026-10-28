import unittest
from typical_sampling_filter import typical_sampling_filter as filt


class TypicalTests(unittest.TestCase):
    def test_reference(self):
        self.assertEqual(filt([0,0,0,0],.6),[True,True,True,False])
        self.assertEqual(filt([2,1,0],.8),[True,True,False])
        self.assertEqual(filt([0,0,0,0],1),[True]*4)
        self.assertEqual(filt([-1000,1000,0],.8),[False,True,False])
        self.assertEqual(filt([5],.5),[True])

    def test_invalid(self):
        cases=[([],0.5),([0]*4097,.5),([0],0),([0],-1),
               ([0],1.1),([0],float('nan')),([0],True),
               ([True],.5),([float('nan')],.5),
               ([float('inf')],.5),([1001],.5),('abc',.5)]
        for args in cases:
            with self.subTest(args=str(args)[:50]):
                with self.assertRaises(ValueError):
                    filt(*args)


if __name__ == "__main__":
    unittest.main()
