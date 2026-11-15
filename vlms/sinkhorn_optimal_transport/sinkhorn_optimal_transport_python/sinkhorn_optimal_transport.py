"""Log-domain Sinkhorn iteration with uniform row and column marginals."""

import math


def _bounded_real(value, name, minimum, maximum):
    if isinstance(value, bool) or not isinstance(value, (float, int)):
        raise ValueError(f"{name} must be a bounded finite real")
    try:
        number = float(value)
    except (ValueError, OverflowError):
        raise ValueError(f"{name} must be a bounded finite real") from None
    if not math.isfinite(number) or not minimum <= number <= maximum:
        raise ValueError(f"{name} must be a bounded finite real")
    return number


def _logsumexp(values):
    high = max(values)
    return high + math.log(math.fsum(math.exp(x - high) for x in values))


def sinkhorn_optimal_transport(costs, regularization=0.5,
                               max_iterations=1000, tolerance=1e-8):
    """Return dense transport plan, cost and convergence diagnostics."""
    if not isinstance(costs, (tuple, list)) or not 1 <= len(costs) <= 32:
        raise ValueError("costs must have 1..32 rows")
    if not isinstance(costs[0], (tuple, list)) or not 1 <= len(costs[0]) <= 32:
        raise ValueError("costs must have 1..32 columns")
    columns = len(costs[0])
    matrix = []
    for row in costs:
        if not isinstance(row, (tuple, list)) or len(row) != columns:
            raise ValueError("cost matrix must be rectangular")
        matrix.append([_bounded_real(v, "cost", 0.0, 20.0) for v in row])

    epsilon = _bounded_real(regularization, "regularization", 0.05, 10.0)
    tol = _bounded_real(tolerance, "tolerance", 1e-12, 0.1)
    if isinstance(max_iterations, bool) or not isinstance(max_iterations, int) or not 1 <= max_iterations <= 2000:
        raise ValueError("max_iterations must be an integer from 1 to 2000")
    rows = len(matrix)
    log_kernel = [[-value / epsilon for value in row] for row in matrix]
    log_row_mass = -math.log(rows)
    log_col_mass = -math.log(columns)
    log_u, log_v = [0.0] * rows, [0.0] * columns

    for iteration in range(1, max_iterations + 1):
        log_u = [
            log_row_mass - _logsumexp([log_kernel[i][j] + log_v[j]
                                      for j in range(columns)])
            for i in range(rows)
        ]
        log_v = [
            log_col_mass - _logsumexp([log_kernel[i][j] + log_u[i]
                                      for i in range(rows)])
            for j in range(columns)
        ]
        plan = [[math.exp(log_u[i] + log_kernel[i][j] + log_v[j])
                 for j in range(columns)] for i in range(rows)]
        deviations = [
            abs(math.fsum(plan[i]) - 1.0 / rows)
            for i in range(rows)
        ] + [
            abs(math.fsum(plan[i][j] for i in range(rows)) - 1.0 / columns)
            for j in range(columns)
        ]
        error = max(deviations)
        if error <= tol:
            break

    transport_cost = math.fsum(plan[i][j] * matrix[i][j]
                               for i in range(rows) for j in range(columns))
    if not math.isfinite(transport_cost) or not math.isfinite(error):
        raise ValueError("numerical error while computing the transport plan")
    return {"plan": plan, "transport_cost": transport_cost,
            "iterations": iteration, "marginal_error": error,
            "converged": error <= tol}
