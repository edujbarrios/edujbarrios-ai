"""Tabular Q-learning from a deterministic sequence of recorded transitions."""

import math


def _bounded_int(value, upper, name):
    if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value < upper:
        raise ValueError(f"{name} must be an integer in [0, {upper})")


def _finite_parameter(value, lower, upper, name, strict_lower=False):
    if isinstance(value, bool) or not isinstance(value, (float, int)):
        raise ValueError(f"{name} must be finite and in range")
    try:
        number = float(value)
    except (OverflowError, ValueError):
        raise ValueError(f"{name} must be finite and in range") from None
    if not math.isfinite(number) or number > upper or (
            number <= lower if strict_lower else number < lower):
        raise ValueError(f"{name} must be finite and in range")
    return number


def tabular_q_learning(episodes, num_states, num_actions,
                       learning_rate=0.5, discount=0.9, passes=1):
    """Return a Q-table and first-action-tie-breaking greedy policy."""
    for value, name in ((num_states, "num_states"), (num_actions, "num_actions")):
        if isinstance(value, bool) or not isinstance(value, int) or not 1 <= value <= 256:
            raise ValueError(f"{name} must be an integer from 1 to 256")
    if isinstance(passes, bool) or not isinstance(passes, int) or not 1 <= passes <= 100:
        raise ValueError("passes must be an integer from 1 to 100")
    rate = _finite_parameter(learning_rate, 0.0, 1.0, "learning_rate", True)
    gamma = _finite_parameter(discount, 0.0, 1.0, "discount")
    if not isinstance(episodes, (tuple, list)) or not 1 <= len(episodes) <= 1024:
        raise ValueError("episodes must contain 1..1024 episodes")

    transitions = []
    for episode in episodes:
        if not isinstance(episode, (tuple, list)) or not episode:
            raise ValueError("each episode must be a nonempty list or tuple")
        for transition in episode:
            if not isinstance(transition, (tuple, list)) or len(transition) != 5:
                raise ValueError("each transition must contain five values")
            state, action, reward, next_state, done = transition
            _bounded_int(state, num_states, "state")
            _bounded_int(action, num_actions, "action")
            _bounded_int(next_state, num_states, "next_state")
            if not isinstance(done, bool):
                raise ValueError("done must be a boolean")
            normalized_reward = _finite_parameter(
                reward, -1_000_000.0, 1_000_000.0, "reward")
            transitions.append((state, action, normalized_reward, next_state, done))
            if len(transitions) > 100_000:
                raise ValueError("at most 100000 transitions are supported")

    q_values = [[0.0] * num_actions for _ in range(num_states)]
    for _ in range(passes):
        for state, action, reward, next_state, done in transitions:
            old = q_values[state][action]
            target = reward if done else reward + gamma * max(q_values[next_state])
            updated = old + rate * (target - old)
            if not math.isfinite(updated):
                raise ValueError("nonfinite Q-value update")
            q_values[state][action] = updated
    policy = [max(range(num_actions), key=lambda a: row[a]) for row in q_values]
    return {"q_values": q_values, "greedy_policy": policy}
