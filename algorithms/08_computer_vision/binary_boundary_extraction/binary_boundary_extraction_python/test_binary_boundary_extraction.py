import unittest
from binary_boundary_extraction import binary_boundary_extraction as boundary


class BoundaryExtractionTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(boundary([1]*9,3,3),[1,1,1,1,0,1,1,1,1])
        self.assertEqual(boundary([0,0,0,0,1,0,0,0,0],3,3),
                         [0,0,0,0,1,0,0,0,0])
        self.assertEqual(boundary([1,1,1,1,0,1,1,1,1],3,3),
                         [1,1,1,1,0,1,1,1,1])
        self.assertEqual(boundary([0]*9,3,3),[0]*9)
        self.assertEqual(boundary([1],1,1),[1])
        self.assertEqual(boundary([0],1,1),[0])
        self.assertEqual(boundary([1,1,1,1],1,4),[1]*4)
        self.assertEqual(boundary([1,1,1,1],4,1),[1]*4)
        self.assertEqual(boundary([1,0,0,1],2,2),[1,0,0,1])
        initial = [1]*9
        boundary(initial,3,3)
        self.assertEqual(initial,[1]*9)
        self.assertEqual(boundary(tuple([1]*9),3,3),[1,1,1,1,0,1,1,1,1])

    def test_maximum(self):
        image = [1]*4096
        result = boundary(image,64,64)
        self.assertEqual(sum(result),252)
        self.assertEqual(result[0],1)
        self.assertEqual(result[63],1)
        self.assertEqual(result[32*64+32],0)
        self.assertEqual(boundary([0]*4096,64,64),[0]*4096)
        checker = [(r+c)%2 for r in range(64) for c in range(64)]
        self.assertEqual(boundary(checker,64,64),checker)

    def test_invalid(self):
        for data,h,w in [
            ([1],0,1),([1],1,0),([1],65,1),([1],1,65),
            ([1],True,1),([1],1,False),([1],1.0,1),
            ([],1,1),([0,1],1,1),([2],1,1),([-1],1,1),
            ([True],1,1),([1.0],1,1),(None,1,1),("1",1,1),
        ]:
            with self.subTest(data=data,h=h,w=w), self.assertRaises(ValueError):
                boundary(data,h,w)


if __name__ == "__main__":
    unittest.main()
