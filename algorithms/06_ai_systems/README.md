# AI Systems & Evaluation

Primitives for evaluating classifiers, probabilistic predictions, retrieval rankings, dataset splits and sequence generation. Each metric highlights a different behavior; use several together rather than treating a single score as complete evidence of model quality.

## Implemented algorithms (28)

| Algorithm | What it does |
| --- | --- |
| [`average_precision_at_k`](average_precision_at_k/README.md) | Summarizes ranked retrieval precision at relevant positions through cutoff k. |
| [`balanced_accuracy`](balanced_accuracy/README.md) | Averages sensitivity and specificity to evaluate binary classification with equal class weight. |
| [`brier_score`](brier_score/README.md) | Measures the squared error of predicted binary-class probabilities. |
| [`cohens_kappa`](cohens_kappa/README.md) | Measures agreement between binary raters after adjusting for expected chance agreement. |
| [`confusion_matrix`](confusion_matrix/README.md) | Counts true and false predictions across binary classes. |
| [`expected_calibration_error`](expected_calibration_error/README.md) | Summarizes discrepancies between prediction confidence and observed accuracy. |
| [`f1_score`](f1_score/README.md) | Combines precision and recall into their harmonic mean. |
| [`f_beta_score`](f_beta_score/README.md) | Balances precision and recall with a configurable beta. |
| [`huber_loss`](huber_loss/README.md) | Combines quadratic and linear regression penalties with a configurable transition. |
| [`markov_beam_search`](markov_beam_search/README.md) | Searches for high-scoring sequences in a Markov model using a limited beam. |
| [`matthews_correlation_coefficient`](matthews_correlation_coefficient/README.md) | Evaluates binary predictions using all four confusion-matrix outcomes. |
| [`mean_absolute_error`](mean_absolute_error/README.md) | Averages absolute residuals to assess the error of numerical regression predictions. |
| [`mean_squared_error`](mean_squared_error/README.md) | Averages squared regression residuals to penalize large errors. |
| [`mean_squared_log_error`](mean_squared_log_error/README.md) | Evaluates nonnegative regression predictions using squared log1p residuals. |
| [`root_mean_squared_error`](root_mean_squared_error/README.md) | Measures regression prediction error in original target units as sqrt(MSE). |
| [`median_absolute_error`](median_absolute_error/README.md) | Reports median absolute regression residual, reducing outlier influence. |
| [`mean_reciprocal_rank`](mean_reciprocal_rank/README.md) | Averages the reciprocal position of the first relevant retrieved result. |
| [`ndcg_at_k`](ndcg_at_k/README.md) | Measures ranking quality while discounting lower positions against an ideal ordering. |
| [`perplexity`](perplexity/README.md) | Expresses average token prediction uncertainty on an exponential scale. |
| [`precision_at_k`](precision_at_k/README.md) | Computes the fraction of the first k retrieved results that are relevant. |
| [`r_squared_score`](r_squared_score/README.md) | Evaluates regression against the target-mean baseline with finite constant-target handling. |
| [`recall_at_k`](recall_at_k/README.md) | Measures the share of relevant results found among the first k retrieved items. |
| [`roc_auc`](roc_auc/README.md) | Summarizes threshold-independent discrimination between positive and negative examples. |
| [`stratified_k_fold`](stratified_k_fold/README.md) | Builds cross-validation folds that preserve class proportions. |
| [`symmetric_mean_absolute_percentage_error`](symmetric_mean_absolute_percentage_error/README.md) | Measures bounded symmetric relative regression error with zero-safe semantics. |
| [`weighted_brier_score`](weighted_brier_score/README.md) | Scores binary probabilistic forecasts using nonnegative sample weights. |
| [`weighted_mean_absolute_error`](weighted_mean_absolute_error/README.md) | Averages absolute errors using nonnegative sample weights. |
| [`top_p_nucleus_filtering`](top_p_nucleus_filtering/README.md) | Restricts sampling candidates to the smallest high-probability token prefix. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
