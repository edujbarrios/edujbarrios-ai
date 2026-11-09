import unittest
from grayscale_contrast_stretching import grayscale_contrast_stretching as stretch


class ContrastStretchingTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(stretch([10,10,15,20],2,2),[0,0,128,255])
        self.assertEqual(stretch([5,6,7,8],1,4),[0,85,170,255])
        self.assertEqual(stretch([99,99],1,2),[99,99])
        self.assertEqual(stretch([0,255],1,2),[0,255])
        self.assertEqual(stretch([5],1,1),[5])
        self.assertEqual(stretch([0,0,0,0],2,2),[0,0,0,0])
        self.assertEqual(stretch((10,20,10),3,1),[0,255,0])
        self.assertEqual(stretch([0,1,2],1,3),[0,128,255])

    def test_maximum(self):
        image = [50] * (64*64-1) + [100]
        result = stretch(image,64,64)
        self.assertEqual(result[:4095],[0]*4095)
        self.assertEqual(result[-1],255)
        self.assertEqual(stretch([120]*4096,64,64),[120]*4096)

    def test_invalid(self):
        for pixels, height, width in [
            ([0],0,1),([0],1,0),([0],65,1),([0],1,65),
            ([0],True,1),([0],1,False),([0],1.0,1),
            ([],1,1),([0,1],1,1),([-1],1,1),([256],1,1),
            ([1.0],1,1),([True],1,1),(None,1,1),("0",1,1),
        ]:
            with self.subTest(pixels=pixels,height=height,width=width):
                with self.assertRaises(ValueError):
                    stretch(pixels,height,width)


if __name__ == "__main__":
    unittest.main()
