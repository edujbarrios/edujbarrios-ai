import unittest
from levenshtein_distance import levenshtein_distance as distance

class LevenshteinTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(distance("kitten","sitting"),3)
        self.assertEqual(distance("sitting","kitten"),3)
        self.assertEqual(distance("","abc"),3)
        self.assertEqual(distance("abc",""),3)
        self.assertEqual(distance("cat","cat"),0)
        self.assertEqual(distance("ab","ba"),2)
        self.assertEqual(distance("A","a"),1)
        self.assertEqual(distance("a"*512,""),512)
    def test_invalid(self):
        for a,b in (("a"*513,""),("a\tb","ab"),
                    ("a\nb","ab"),("é","e"),("\0",""),
                    ("\x01",""),(None,"a"),(1,"a")):
            with self.subTest(a=a,b=b),self.assertRaises(ValueError):
                distance(a,b)

if __name__=="__main__":
    unittest.main()
