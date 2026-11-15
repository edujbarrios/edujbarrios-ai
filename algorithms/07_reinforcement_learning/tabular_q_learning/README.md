# Tabular Q-Learning from Recorded Transitions

## Why it belongs in this repository

**Reinforcement learning** seeks decisions that maximize cumulative rewards.
Tabular Q-learning uses temporal-difference bootstrapping to learn action
values without a model of environment transitions (Watkins & Dayan, 1992).
This entry introduces off-policy value updates and provides a reference
baseline for small, discrete environments.

## Algorithm

For a transition `(state, action, reward, next_state, done)`, update

```text
target = reward                           if done
         reward + gamma * max_a Q[next_state, a] otherwise

Q[state, action] += learning_rate * (target - Q[state, action])
```

Initialize `Q` to zero. Replay transitions in **input order** for the
requested number of passes; no random action selection occurs. Return
the learned Q-table plus a greedy policy selecting the action with the
highest estimated value for each state (lowest action index on ties).

## Contract

- `episodes` is a nonempty list/tuple of up to 1024 **nonempty**
  episodes. Each episode is a list/tuple of 5-tuples
  `(state, action, reward, next_state, done)`. Total transition count
  must not exceed 100,000.
- `state`, `next_state` are zero-based integers in
  `[0, num_states)`; `action` is in `[0, num_actions)`; none
  may be booleans. `done` is a bool; the next state is checked
  even when terminal, but not bootstrapped from.
- `num_states`, `num_actions` are positive integers up to 256.
  `learning_rate` is finite in `(0, 1]`, `discount` finite in
  `[0, 1]`, and `passes` an integer from 1 through 100.
- Rewards must be finite reals (not bool) with magnitude <= 1,000,000.
  Errors raise `ValueError`, with no input mutation.
- Returns `{"q_values": [[...], ...], "greedy_policy": [...]}`,
  with one Q-value per state/action and zero-based policy actions.
  Updates use binary64 and reject nonfinite intermediate values.

## Worked example

```python
episodes = [[(0, 0, 1, 1, False), (1, 0, 2, 1, True)]]
result = tabular_q_learning(
    episodes, num_states=2, num_actions=2,
    learning_rate=1.0, discount=0.5, passes=2
)
# result["q_values"] == [[2.0, 0.0], [2.0, 0.0]]
# result["greedy_policy"] == [0, 0]
```

On pass one the first action sees the initially zero next-state value,
then the terminal transition updates to 2; on pass two that learned
value is bootstrapped into the first action.

## Complexity

For `T` recorded transitions, `P` passes, `S` states and
`A` actions: **O(P T A + S A)** time due to `max` over actions,
and **O(T + S A)** auxiliary storage (validated transitions and Q-table).

## Implementations

| Language | Folder |
| --- | --- |
| Python | [tabular_q_learning_python](tabular_q_learning_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover bootstrapping, terminal transitions, negative rewards,
deterministic policy ties, reproducibility, validation and immutability.

## Limitations

This **does not interact with an environment**, choose exploratory actions,
or guarantee learning from an arbitrary fixed log. Without sufficient
state/action coverage, learning can be biased or incomplete. Replaying
a fixed dataset for a finite number of passes is an educational version,
not a convergence guarantee; offline RL can require additional safeguards.
The state/action spaces must be discrete and small.

## References

Watkins, C. J. C. H., & Dayan, P. (1992). Q-learning. *Machine Learning,
8*, 279–292. https://doi.org/10.1007/BF00992698
