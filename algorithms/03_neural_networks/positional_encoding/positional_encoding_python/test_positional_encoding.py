import math
import unittest
from positional_encoding import positional_encoding

class PositionalEncodingTests(unittest.TestCase):
    def test_reference_values(self):
        matrix=positional_encoding(2,4)
        self.assertEqual(matrix[0],[0.0,1.0,0.0,1.0])
        self.assertAlmostEqual(matrix[1][0],math.sin(1))
        self.assertAlmostEqual(matrix[1][1],math.cos(1))
        self.assertAlmostEqual(matrix[1][2],math.sin(0.01))
        self.assertAlmostEqual(matrix[1][3],math.cos(0.01))
        self.assertEqual(positional_encoding(1,1),[[0.0]])
        self.assertEqual(positional_encoding(2,3)[0],[0.0,1.0,0.0])
    def test_invalid_and_bounds(self):
        for length,width in ((0,4),(1,0),(-1,2),(4097,1),(1,1025),
                             (True,2),(1,True),(2.0,4)):
            with self.subTest(length=length,width=width):
                with self.assertRaises(ValueError):
                    positional_encoding(length,width)
        m=positional_encoding(4,7)
        self.assertTrue(all(math.isfinite(x) and -1<=x<=1 for row in m for x in row))

if __name__=="__main__":
    unittest.main()
