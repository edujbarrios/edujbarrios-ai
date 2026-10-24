import unittest
from top_p_nucleus_filtering import top_p_nucleus_filtering as nucleus

class TopPTests(unittest.TestCase):
    def test_prefixes_and_ties(self):
        p=[.4,.3,.2,.1]
        self.assertEqual(nucleus(p,.6),[0,1])
        self.assertEqual(nucleus(p,.85),[0,1,2])
        self.assertEqual(nucleus(p,.95),[0,1,2,3])
        self.assertEqual(nucleus([.5,.5,0],.5),[0])
        self.assertEqual(nucleus([.5,.5,0],.75),[0,1])
        self.assertEqual(nucleus([.5,.5,0],1),[0,1])
        self.assertEqual(nucleus([1],1),[0])
    def test_invalid(self):
        cases=(([],.5),([.7,.7],.5),([1.1,-.1],.5),
               ([float("nan"),1],.5),([.5,.5],0),
               ([.5,.5],1.1),([True,0],.5),([1],True))
        for a,p in cases:
            with self.subTest(a=a,p=p),self.assertRaises(ValueError):
                nucleus(a,p)

if __name__=="__main__":unittest.main()
