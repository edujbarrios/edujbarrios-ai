"""Aho–Corasick substring matching with output links and exact offsets."""

from collections import deque


def aho_corasick_search(text, patterns):
    """Return (pattern_index, zero_based_start) for all overlapping matches."""
    if not isinstance(text, str) or len(text) > 100_000:
        raise ValueError("text must be a string of at most 100000 code points")
    if not isinstance(patterns, (tuple, list)) or not 1 <= len(patterns) <= 256:
        raise ValueError("patterns must contain 1..256 nonempty strings")

    total = 0
    for pattern in patterns:
        if not isinstance(pattern, str) or not pattern:
            raise ValueError("each pattern must be a nonempty string")
        total += len(pattern)
        if total > 8192:
            raise ValueError("total pattern length must not exceed 8192")

    nodes = [{"next": {}, "fail": 0, "terms": [], "output_link": 0}]
    for pattern_index, pattern in enumerate(patterns):
        state = 0
        for character in pattern:
            children = nodes[state]["next"]
            if character not in children:
                children[character] = len(nodes)
                nodes.append({"next": {}, "fail": 0, "terms": [], "output_link": 0})
            state = children[character]
        nodes[state]["terms"].append(pattern_index)

    queue = deque(nodes[0]["next"].values())
    while queue:
        state = queue.popleft()
        for character, child in nodes[state]["next"].items():
            parent_failure = nodes[state]["fail"]
            while parent_failure and character not in nodes[parent_failure]["next"]:
                parent_failure = nodes[parent_failure]["fail"]
            failure = nodes[parent_failure]["next"].get(character, 0)
            nodes[child]["fail"] = failure
            nodes[child]["output_link"] = (
                failure if nodes[failure]["terms"] else nodes[failure]["output_link"]
            )
            queue.append(child)

    matches = []  # (start_offset, pattern_index) for natural sorted ordering.
    state = 0
    for end, character in enumerate(text):
        while state and character not in nodes[state]["next"]:
            state = nodes[state]["fail"]
        state = nodes[state]["next"].get(character, 0)
        output_state = state
        while output_state:
            for pattern_index in nodes[output_state]["terms"]:
                matches.append((end - len(patterns[pattern_index]) + 1,
                                pattern_index))
                if len(matches) > 200_000:
                    raise ValueError("more than 200000 matches")
            output_state = nodes[output_state]["output_link"]

    return [(pattern_index, start) for start, pattern_index in sorted(matches)]
