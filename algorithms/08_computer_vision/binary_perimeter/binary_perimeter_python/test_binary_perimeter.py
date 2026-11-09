import unittest
from binary_perimeter import binary_perimeter as perimeter


class BinaryPerimeterTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(perimeter([1],1,1),4)
        self.assertEqual(perimeter([0],1,1),0)
        self.assertEqual(perimeter([1,1,1,1],2,2),8)
        self.assertEqual(perimeter([0,0,0,0],2,2),0)
        self.assertEqual(perimeter([1,0,1],1,3),8)
        self.assertEqual(perimeter([1,1,1],1,3),8)
        self.assertEqual(perimeter([1,1,1],3,1),8)
        self.assertEqual(perimeter([1,1,1,1,0,1,1,1,1],3,3),16)
        self.assertEqual(perimeter([0,0,0,0,1,0,0,0,0],3,3),4)
        self.assertEqual(perimeter((1,0,0,1),2,2),8)

    def test_maximum(self):
        self.assertEqual(perimeter([1]*4096,64,64),256)
        checker = [(r+c)%2 for r in range(64) for c in range(64)]
        self.assertEqual(perimeter(checker,64,64),8192)
        self.assertEqual(perimeter([0]*4096,64,64),0)

    def test_invalid(self):
        for pixels,h,w in [
            ([1],0,1),([1],1,0),([1],65,1),([1],1,65),
            ([1],True,1),([1],1,False),([1],1.0,1),
            ([],1,1),([0,1],1,1),([2],1,1),([-1],1,1),
            ([True],1,1),([1.0],1,1),(None,1,1),("1",1,1),
        ]:
            with self.subTest(pixels=pixels,h=h,w=w), self.assertRaises(ValueError):
                perimeter(pixels,h,w)


if __name__ == "__main__":
    unittest.main()
