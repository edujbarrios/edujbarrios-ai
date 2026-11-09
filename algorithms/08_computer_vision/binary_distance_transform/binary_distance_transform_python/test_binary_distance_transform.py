import unittest
from binary_distance_transform import binary_distance_transform as distance


class BinaryDistanceTransformTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(distance([1,1,1,1,0,1,1,1,1],3,3),
                         [2,1,2,1,0,1,2,1,2])
        self.assertEqual(distance([0,1,1,1],2,2),[0,1,1,2])
        self.assertEqual(distance([1,1,1,1],2,2),[4,4,4,4])
        self.assertEqual(distance([0,0,0,0],2,2),[0,0,0,0])
        self.assertEqual(distance([0],1,1),[0])
        self.assertEqual(distance([1],1,1),[2])
        self.assertEqual(distance([1,0,1,1],1,4),[1,0,1,2])
        self.assertEqual(distance([1,0,1,1],4,1),[1,0,1,2])
        self.assertEqual(distance((1,0,1),1,3),[1,0,1])

    def test_maximum(self):
        mask = [1] * 4096
        mask[0] = 0
        result = distance(mask,64,64)
        self.assertEqual(result[0],0)
        self.assertEqual(result[1],1)
        self.assertEqual(result[64],1)
        self.assertEqual(result[4095],126)
        self.assertEqual(distance([1]*4096,64,64),[128]*4096)

    def test_invalid(self):
        for pixels,h,w in [
            ([1],0,1),([1],1,0),([1],65,1),([1],1,65),
            ([1],True,1),([1],1,1.0),([1],1,False),
            ([1,0],1,1),([0,2],1,2),([True],1,1),
            ([1.0],1,1),([None],1,1),(None,1,1),
            ("01",1,2),([],1,1),
        ]:
            with self.subTest(pixels=pixels,h=h,w=w):
                with self.assertRaises(ValueError):
                    distance(pixels,h,w)


if __name__ == "__main__":
    unittest.main()
