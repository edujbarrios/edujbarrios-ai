import unittest
from hamming_radius_search import hamming_radius_search as search

class HammingRadiusTests(unittest.TestCase):
    def test_radii_and_order(self):
        values=[3,1,7,15,2]
        self.assertEqual(search(3,values,1),[0,1,2,4])
        self.assertEqual(search(3,values,0),[0])
        self.assertEqual(search(3,values,30),[0,1,2,4,3])
        self.assertEqual(search(3,[3,3,1],0),[0,1])
        self.assertEqual(search(0,[],0),[])
        self.assertEqual(search(0,[0,(1<<30)-1],30),[0,1])
    def test_invalid(self):
        cases=((3,[3,1<<30],0),(1<<30,[3],0),
               (3,[3],31),(3,[3],-1),(3,[3],True),
               (True,[3],1),(3,[True],1),
               (3,[3.0],1),(3,[3],1.5))
        for q,values,radius in cases:
            with self.subTest(query=q,values=values,radius=radius):
                with self.assertRaises(ValueError):
                    search(q,values,radius)

if __name__=="__main__":
    unittest.main()
