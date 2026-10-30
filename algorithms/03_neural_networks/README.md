# Neural Networks

A collection of neural network building blocks: nonlinearities, objectives, parameter updates, normalization, forward and backward propagation, and transformer attention. Individual entries expose the relevant equations and computational trade-offs.

## Implemented algorithms (35)

| Algorithm | What it does |
| --- | --- |
| [`adam_optimizer`](adam_optimizer/README.md) | Combines momentum and adaptive squared-gradient estimates to update parameters. |
| [`adamw_optimizer`](adamw_optimizer/README.md) | Applies Adam-style adaptive updates with decoupled weight decay. |
| [`batch_normalization`](batch_normalization/README.md) | Normalizes activations using batch statistics and learned scale and offset. |
| [`binary_cross_entropy`](binary_cross_entropy/README.md) | Measures error between binary labels and predicted probabilities. |
| [`binary_cross_entropy_with_logits`](binary_cross_entropy_with_logits/README.md) | Computes stable binary cross-entropy from raw logits and soft targets. |
| [`binary_focal_loss`](binary_focal_loss/README.md) | Down-weights easier binary examples so training emphasizes difficult ones. |
| [`causal_attention`](causal_attention/README.md) | Prevents attention from looking ahead at future sequence positions. |
| [`cross_entropy`](cross_entropy/README.md) | Measures how well predicted categorical probabilities match target labels. |
| [`cosine_annealing_learning_rate`](cosine_annealing_learning_rate/README.md) | Smoothly decreases a learning rate between specified bounds across a fixed horizon. |
| [`dense_forward`](dense_forward/README.md) | Computes the affine transformation in a fully connected layer. |
| [`dense_relu_backprop`](dense_relu_backprop/README.md) | Propagates gradients through a dense layer followed by ReLU. |
| [`elu`](elu/README.md) | Applies ELU with a linear positive branch and exponentially saturating negative branch. |
| [`gelu`](gelu/README.md) | Applies a smooth Gaussian-error-based nonlinear activation. |
| [`hinge_loss`](hinge_loss/README.md) | Penalizes violations of the classification margin. |
| [`huber_loss`](huber_loss/README.md) | Combines quadratic behavior for small errors with linear behavior for outliers. |
| [`label_smoothed_cross_entropy`](label_smoothed_cross_entropy/README.md) | Uses softened class targets to regularize categorical training. |
| [`layer_normalization`](layer_normalization/README.md) | Normalizes each example across its feature dimension. |
| [`leaky_relu`](leaky_relu/README.md) | Preserves a small negative slope in a ReLU-style activation. |
| [`log_cosh_loss`](log_cosh_loss/README.md) | Smooth, robust regression loss with numerically stable mean evaluation. |
| [`log_softmax`](log_softmax/README.md) | Converts raw logits into stable normalized log-probabilities. |
| [`masked_attention`](masked_attention/README.md) | Excludes disallowed positions when computing attention weights. |
| [`momentum_sgd`](momentum_sgd/README.md) | Accumulates momentum to smooth successive gradient-based updates. |
| [`positional_encoding`](positional_encoding/README.md) | Adds position-dependent signals to otherwise order-agnostic token representations. |
| [`relu`](relu/README.md) | Returns the positive part of each input to introduce nonlinearity. |
| [`rmsprop_optimizer`](rmsprop_optimizer/README.md) | Adapts step sizes using moving averages of squared gradients. |
| [`rotary_positional_embedding`](rotary_positional_embedding/README.md) | Encodes relative position information by rotating embedding pairs. |
| [`scaled_dot_product_attention`](scaled_dot_product_attention/README.md) | Weights values by softmax-normalized query–key similarities. |
| [`sigmoid`](sigmoid/README.md) | Maps real-valued scores into the interval between zero and one. |
| [`sparsemax`](sparsemax/README.md) | Projects logits onto the probability simplex, allowing exact zero weights. |\n| [`softmax`](softmax/README.md) | Converts a vector of scores into a normalized categorical distribution. |
| [`softplus`](softplus/README.md) | Computes a smooth activation and its stable sigmoid derivative. |
| [`softsign`](softsign/README.md) | Applies a smooth bounded x/(1+abs(x)) activation. |
| [`swish`](swish/README.md) | Applies a smooth sigmoid-gated nonlinear activation. |
| [`temperature_softmax`](temperature_softmax/README.md) | Controls distribution sharpness by rescaling logits before softmax. |
| [`two_layer_binary_classifier`](two_layer_binary_classifier/README.md) | Demonstrates end-to-end prediction and learning with a small neural classifier. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
