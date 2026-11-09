import unittest
from boyer_moore_horspool_search import boyer_moore_horspool_search as search


class HorspoolTests(unittest.TestCase):
    def test_matches(self):
        self.assertEqual(search("ABAAABCD","ABC"),[4])
        self.assertEqual(search("aaaa","aa"),[0,1,2])
        self.assertEqual(search("abababa","aba"),[0,2,4])
        self.assertEqual(search("ABCABCABC","ABC"),[0,3,6])
        self.assertEqual(search("mississippi","issi"),[1,4])
        self.assertEqual(search("abc","XYZ"),[])
        self.assertEqual(search("abc","ABC"),[])
        self.assertEqual(search("ab","abc"),[])
        self.assertEqual(search("~a~a","~a"),[0,2])
        self.assertEqual(search("a a a","a "),[0,2])

    def test_boundaries(self):
        self.assertEqual(search("abc",""),[0,1,2,3])
        self.assertEqual(search("",""),[0])
        self.assertEqual(search("","a"),[])
        self.assertEqual(search("a"*512,"aa"),list(range(511)))
        self.assertEqual(search("a"*512,"a"*512),[0])
        self.assertEqual(search("a"*512,""),list(range(513)))
        self.assertEqual(search("a","a"),[0])

    def test_invalid(self):
        for a,b in [
            ("a"*513,"a"),("a","b"*513),
            ("é","e"),("a\tb","a"),("a\0b","a"),
            ("a","\n"),(None,"a"),("a",1),(b"ab","a"),
        ]:
            with self.subTest(a=str(a)[:20],b=str(b)[:20]):
                with self.assertRaises(ValueError):
                    search(a,b)


if __name__ == "__main__":
    unittest.main()
