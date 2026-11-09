import unittest
from binary_projection_profiles import binary_projection_profiles as profiles


class BinaryProfilesTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(profiles([1,0,1,0,1,1],2,3),[2,2,1,1,2])
        self.assertEqual(profiles([1,0,1,1],1,4),[3,1,0,1,1])
        self.assertEqual(profiles([1,0,1,1],4,1),[1,0,1,1,3])
        self.assertEqual(profiles([0,0,0,0],2,2),[0,0,0,0])
        self.assertEqual(profiles([1,1,1,1],2,2),[2,2,2,2])
        self.assertEqual(profiles([0],1,1),[0,0])
        self.assertEqual(profiles([1],1,1),[1,1])
        self.assertEqual(profiles((1,0,0,1),2,2),[1,1,1,1])
        image = [1,0,0,0,1,0]
        original = image.copy()
        self.assertEqual(profiles(image,3,2),[1,0,1,2,0])
        self.assertEqual(image,original)

    def test_limits(self):
        image = [1]*4096
        self.assertEqual(profiles(image,64,64),[64]*128)
        self.assertEqual(profiles([0]*4096,64,64),[0]*128)
        checker = [(r+c)%2 for r in range(64) for c in range(64)]
        self.assertEqual(profiles(checker,64,64),[32]*128)
        mixed = [0]*4096
        mixed[0] = mixed[4095] = 1
        out = profiles(mixed,64,64)
        self.assertEqual(out[0],1)
        self.assertEqual(out[63],1)
        self.assertEqual(out[64],1)
        self.assertEqual(out[127],1)
        self.assertEqual(sum(out[:64]),2)
        self.assertEqual(sum(out[64:]),2)

    def test_invalid(self):
        for pixels,h,w in [
            ([0],0,1),([0],1,0),([0],65,1),([0],1,65),
            ([0],True,1),([0],1,False),([0],1.0,1),
            ([],1,1),([0,1],1,1),([2],1,1),([-1],1,1),
            ([True],1,1),([1.0],1,1),(None,1,1),("0",1,1),
        ]:
            with self.subTest(pixels=pixels,h=h,w=w):
                with self.assertRaises(ValueError):
                    profiles(pixels,h,w)


if __name__ == "__main__":
    unittest.main()
