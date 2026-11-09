"""Macro-averaged F1 across all explicitly declared classes."""


def macro_f1_score(actual, predicted, classes):
    """Return unweighted mean one-vs-rest F1, absent-class F1 defined as 0."""
    if type(classes) is not int or not 2 <= classes <= 16:
        raise ValueError("classes must be an integer in 2..16")
    if (not isinstance(actual, (list, tuple))
            or not isinstance(predicted, (list, tuple))
            or not 1 <= len(actual) <= 512 or len(actual) != len(predicted)):
        raise ValueError("expected nonempty aligned label sequences")
    truth = [0] * classes
    guesses = [0] * classes
    hits = [0] * classes
    for a, p in zip(actual, predicted):
        if (type(a) is not int or type(p) is not int or
                not 0 <= a < classes or not 0 <= p < classes):
            raise ValueError("labels must be integer IDs within classes")
        truth[a] += 1
        guesses[p] += 1
        if a == p:
            hits[a] += 1
    return sum(2.0 * hits[c] / (truth[c] + guesses[c])
               for c in range(classes)
               if truth[c] + guesses[c] > 0) / classes
