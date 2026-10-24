import unittest
from astar_shortest_path import astar_shortest_path as astar

class AStarTests(unittest.TestCase):
    def test_paths(self):
        graph=[[0,2,5,0],[0,0,1,10],[0,0,0,2],[0,0,0,0]]
        h=[4,3,2,0]
        self.assertEqual(astar(graph,h,0,3),(5,[0,1,2,3]))
        self.assertEqual(astar(graph,[0]*4,0,3),(5,[0,1,2,3]))
        self.assertEqual(astar(graph,h,3,3),(0,[3]))
        self.assertEqual(astar([[0,1,0],[0,0,0],[0,0,0]],[0,0,0],0,2),(-1,[]))
        self.assertEqual(astar([[0,1,1],[0,0,1],[0,0,0]],[0,0,0],0,2),(1,[0,2]))
    def test_invalid(self):
        graph=[[0,2,5,0],[0,0,1,10],[0,0,0,2],[0,0,0,0]]
        for w,h,s,t in ((graph,[10,3,2,0],0,3),
                        (graph,[4,3,2,0],0,2),
                        (graph,[4,3,2,0],4,3),
                        ([[0,-1],[0,0]],[0,0],0,1),
                        ([[0,1],[0,float("nan")]],[0,0],0,1),
                        ([[0,1]],[0],0,0)):
            with self.subTest(w=w,h=h,s=s,t=t),self.assertRaises(ValueError):
                astar(w,h,s,t)

if __name__=="__main__":unittest.main()
