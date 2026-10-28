import math
import unittest
from rms_normalization import rms_normalization as rms


class RMSNormTests(unittest.TestCase):
    def test_reference(self):
        actual=rms([1,1],[1,2],1)
        self.assertAlmostEqual(actual[0],1/math.sqrt(2))
        self.assertAlmostEqual(actual[1],2/math.sqrt(2))
        self.assertEqual(rms([0,0],[1,2],1e-8),[0,0])
        self.assertAlmostEqual(rms([1],[1],1)[0],1/math.sqrt(2))
        self.assertAlmostEqual(rms([3,4],[1,1],1e-8)[0],
                               3/math.sqrt(12.5+1e-8))

    def test_invalid(self):
        x=[1,1]; g=[1,2]
        cases=[([],[],1),(x,g[:-1],1),(x,g,0),
               (x,g,1e-9),(x,g,2),(x,g,float('nan')),
               (x,g,True),([1,float('nan')],g,1),
               (x,[1,1001],1),([True,1],g,1),
               (x,g+[1],1),('11',g,1)]
        for args in cases:
            with self.subTest(args=str(args)[:55]):
                with self.assertRaises(ValueError):
                    rms(*args)


if __name__=="__main__":
    unittest.main()
