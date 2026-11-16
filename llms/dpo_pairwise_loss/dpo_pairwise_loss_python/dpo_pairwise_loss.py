"""Stable DPO pairwise objective from sequence log probabilities."""

import math


def _log_probabilities(values, name):
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 4096:
        raise ValueError(f"{name} must contain 1..4096 sequence log probabilities")
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (float, int)):
            raise ValueError(f"{name} must contain finite log probabilities")
        try:
            score = float(value)
        except (ValueError, OverflowError):
            raise ValueError(f"{name} must contain finite log probabilities") from None
        if not math.isfinite(score) or not -1_000_000 <= score <= 0:
            raise ValueError(f"{name} values must be finite and in [-1000000, 0]")
        result.append(score)
    return result


def _softplus_negative(margin):
    """Evaluate log(1 + exp(-margin)) without exponential overflow."""
    if margin >= 0:
        return math.log1p(math.exp(-margin))
    return -margin + math.log1p(math.exp(margin))


def dpo_pairwise_loss(policy_chosen, policy_rejected,
                      reference_chosen, reference_rejected, beta=0.1):
    """Return DPO losses, their mean, and positive-margin fraction."""
    if isinstance(beta, bool) or not isinstance(beta, (float, int)):
        raise ValueError("beta must be finite and in (0,100]")
    try:
        beta = float(beta)
    except (ValueError, OverflowError):
        raise ValueError("beta must be finite and in (0,100]") from None
    if not math.isfinite(beta) or not 0 < beta <= 100:
        raise ValueError("beta must be finite and in (0,100]")

    pc = _log_probabilities(policy_chosen, "policy_chosen")
    pr = _log_probabilities(policy_rejected, "policy_rejected")
    rc = _log_probabilities(reference_chosen, "reference_chosen")
    rr = _log_probabilities(reference_rejected, "reference_rejected")
    if not len(pc) == len(pr) == len(rc) == len(rr):
        raise ValueError("all four log-probability arrays must have equal length")

    losses = []
    wins = 0
    for chosen, rejected, ref_chosen, ref_rejected in zip(pc, pr, rc, rr):
        margin = beta * ((chosen - rejected) - (ref_chosen - ref_rejected))
        losses.append(_softplus_negative(margin))
        wins += margin > 0
    return {
        "losses": losses,
        "mean_loss": math.fsum(losses) / len(losses),
        "preference_accuracy": wins / len(losses),
    }
