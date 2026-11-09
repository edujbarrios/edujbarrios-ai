import unittest
from binary_hole_filling import binary_hole_filling as fill


class BinaryHoleFillingTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(fill([1,1,1,1,0,1,1,1,1],3,3),[1]*9)
        self.assertEqual(fill([0,1,1,1,0,1,1,1,1],3,3),
                         [0,1,1,1,1,1,1,1,1])
        self.assertEqual(fill([0,0,0,0],2,2),[0]*4)
        self.assertEqual(fill([1,1,1,1],2,2),[1]*4)
        self.assertEqual(fill([0],1,1),[0])
        self.assertEqual(fill([1],1,1),[1])
        self.assertEqual(fill([1,0,1,0],1,4),[1,0,1,0])
        self.assertEqual(fill([1,0,1,0],4,1),[1,0,1,0])
        donut = [0,0,0,0,0, 0,1,1,1,0, 0,1,0,1,0,
                 0,1,1,1,0, 0,0,0,0,0]
        result = donut.copy()
        result[12] = 1
        self.assertEqual(fill(donut,5,5),result)
        channel = [0,0,0, 1,0,1, 1,0,1]
        self.assertEqual(fill(channel,3,3),channel)

    def test_large(self):
        mask = [1]*4096
        mask[32*64+32] = 0
        self.assertEqual(fill(mask,64,64),[1]*4096)
        self.assertEqual(fill([0]*4096,64,64),[0]*4096)

    def test_invalid(self):
        for pixels,h,w in [
            ([1],0,1),([1],1,0),([1],65,1),([1],1,65),
            ([1],True,1),([1],1,1.0),([1],1,False),
            ([0,1],1,1),([0,2],1,2),([True],1,1),
            ([1.0],1,1),([None],1,1),(None,1,1),("01",1,2),([],1,1),
        ]:
            with self.subTest(pixels=pixels,h=h,w=w), self.assertRaises(ValueError):
                fill(pixels,h,w)


if __name__ == "__main__":
    unittest.main()
