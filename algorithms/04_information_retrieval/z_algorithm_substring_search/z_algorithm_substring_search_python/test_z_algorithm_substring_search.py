import unittest
from z_algorithm_substring_search import z_algorithm_substring_search as search


class ZSearchTests(unittest.TestCase):
    def test_examples(self):
        self.assertEqual(search("banana","ana"),[1,3])
        self.assertEqual(search("aaaa","aa"),[0,1,2])
        self.assertEqual(search("abcabc","abc"),[0,3])
        self.assertEqual(search("abc","xyz"),[])
        self.assertEqual(search("abc","ABC"),[])
        self.assertEqual(search("ab","abc"),[])
        self.assertEqual(search("mississippi","issi"),[1,4])
        self.assertEqual(search("a a a","a "),[0,2])
        self.assertEqual(search("~!~!","~!"),[0,2])
        self.assertEqual(search("aba","aba"),[0])

    def test_boundaries(self):
        self.assertEqual(search("abc",""),[0,1,2,3])
        self.assertEqual(search("",""),[0])
        self.assertEqual(search("","a"),[])
        self.assertEqual(search("a"*512,"aa"),list(range(511)))
        self.assertEqual(search("a"*512,"a"*512),[0])
        self.assertEqual(search("a"*512,""),list(range(513)))
        self.assertEqual(search("a","a"),[0])

    def test_invalid(self):
        for text, pattern in [
            ("a"*513,"a"),("a","b"*513),("é","e"),("a\tb","a"),
            ("a\0b","a"),("a","\n"),(None,"a"),("a",1),(b"abc","a"),
        ]:
            with self.subTest(text=str(text)[:20],pattern=str(pattern)[:20]):
                with self.assertRaises(ValueError):
                    search(text,pattern)


if __name__ == "__main__":
    unittest.main()
