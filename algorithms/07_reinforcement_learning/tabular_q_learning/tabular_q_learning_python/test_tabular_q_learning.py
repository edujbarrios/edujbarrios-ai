"""Reference Q-learning updates, terminal handling, and invalid inputs."""

import math
import unittest
from tabular_q_learning import tabular_q_learning as learn


class TabularQLearningTests(unittest.TestCase):
    def test_bootstrapping_over_recorded_transitions(self):
        episodes = [[(0, 0, 1, 1, False), (1, 0, 2, 1, True)]]
        result = learn(episodes, 2, 2, learning_rate=1, discount=0.5, passes=2)
        self.assertEqual(result["q_values"], [[2.0, 0.0], [2.0, 0.0]])
        self.assertEqual(result["greedy_policy"], [0, 0])
        self.assertEqual(episodes, [[(0, 0, 1, 1, False), (1, 0, 2, 1, True)]])

    def test_terminal_and_negative_rewards(self):
        episodes = [[(0, 0, -1, 0, True), (0, 1, 1, 0, True)]]
        result = learn(episodes, 1, 2, learning_rate=1, discount=1)
        self.assertEqual(result["q_values"], [[-1.0, 1.0]])
        self.assertEqual(result["greedy_policy"], [1])
        self.assertEqual(learn(episodes, 1, 2), learn(episodes, 1, 2))
        self.assertEqual(learn([[(0, 0, 0, 0, True)]], 1, 2)["greedy_policy"], [0])

    def test_invalid(self):
        valid = [[(0, 0, 1, 0, True)]]
        bad_kwargs = [
            {"num_states": 0}, {"num_states": True}, {"num_actions": 257},
            {"num_actions": 0}, {"learning_rate": 0},
            {"learning_rate": 1.1}, {"discount": -0.1}, {"discount": math.nan},
            {"passes": 0}, {"passes": True},
        ]
        for overrides in bad_kwargs:
            with self.subTest(overrides=overrides), self.assertRaises(ValueError):
                params = {"num_states": 1, "num_actions": 1}
                params.update(overrides)
                learn(valid, **params)
        bad_episodes = [
            [], [[]], [[(0, 0, 1, 0)]], [[(0, 0, 1, 0, 0)]],
            [[(True, 0, 1, 0, True)]], [[(1, 0, 1, 0, True)]],
            [[(0, 1, 1, 0, True)]], [[(0, 0, math.inf, 0, True)]],
            [[(0, 0, True, 0, True)]], [[(0, 0, 1, 1, True)]],
            [[(0, 0, 1, 0, "yes")]], None,
        ]
        for episodes in bad_episodes:
            with self.subTest(episodes=episodes), self.assertRaises(ValueError):
                learn(episodes, 1, 1)


if __name__ == "__main__":
    unittest.main()
