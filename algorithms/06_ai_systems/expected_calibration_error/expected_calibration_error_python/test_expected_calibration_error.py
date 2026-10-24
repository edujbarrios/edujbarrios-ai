import unittest
from expected_calibration_error import expected_calibration_error as ece

class ExpectedCalibrationTests(unittest.TestCase):
    def test_bins_and_confidence(self):
        self.assertAlmostEqual(ece([.9,.8,.2,.6],[1,0,0,1],2),.025)
        self.assertAlmostEqual(ece([.9,.8,.2,.6],[1,0,0,1],4),.225)
        self.assertEqual(ece([1,0],[1,0],10),0)
        self.assertEqual(ece([.5,.5],[0,1],1),0)
        self.assertEqual(ece([1],[0],100),1)
    def test_invalid(self):
        cases=(([],[],2),([.5],[],2),([.5],[1],0),
               ([.5],[1],101),([1.1],[1],2),([-.1],[0],2),
               ([float("nan")],[1],2),([.5],[2],2),
               ([True],[1],2),([.5],[True],2),([.5],[1],True))
        for case in cases:
            with self.subTest(args=case),self.assertRaises(ValueError):
                ece(*case)

if __name__=="__main__":unittest.main()
