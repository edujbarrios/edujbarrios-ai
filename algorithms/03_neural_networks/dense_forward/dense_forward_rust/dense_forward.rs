//! Dense affine forward inference followed by ReLU.

/// Compute relu(weights @ input + bias). Weights are row-major.
pub fn dense_forward(
    input: &[f64], weights: &[f64], bias: &[f64],
    output_dim: usize,
) -> Result<Vec<f64>, &'static str> {
    if input.is_empty() || output_dim == 0 {
        return Err("positive input and output dimensions required");
    }
    let expected = output_dim.checked_mul(input.len()).ok_or("shape overflow")?;
    if weights.len() != expected || bias.len() != output_dim {
        return Err("weights or biases have incompatible shape");
    }
    if !input.iter().chain(weights.iter()).chain(bias.iter()).all(|x| x.is_finite()) {
        return Err("inputs must be finite");
    }

    let mut output = Vec::with_capacity(output_dim);
    for j in 0..output_dim {
        let mut total = 0.0_f64;
        for i in 0..input.len() {
            let product = weights[j * input.len() + i] * input[i];
            if !product.is_finite() {
                return Err("multiplication overflow");
            }
            total += product;
            if !total.is_finite() {
                return Err("summation overflow");
            }
        }
        let z = total + bias[j];
        if !z.is_finite() {
            return Err("bias addition overflow");
        }
        output.push(z.max(0.0));
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::dense_forward as forward;

    #[test]
    fn regular_and_one_neuron() {
        assert_eq!(forward(&[2.0,-1.0], &[1.0,1.0,-1.0,2.0], &[0.0,1.0],2),
                   Ok(vec![1.0,0.0]));
        assert_eq!(forward(&[1.0], &[3.0], &[2.0],1),Ok(vec![5.0]));
        assert_eq!(forward(&[-1.0], &[3.0], &[2.0],1),Ok(vec![0.0]));
    }
    #[test]
    fn invalid() {
        assert!(forward(&[],&[],&[],1).is_err());
        assert!(forward(&[1.0],&[],&[],0).is_err());
        assert!(forward(&[1.0,2.0],&[1.0],&[0.0],1).is_err());
        assert!(forward(&[1.0],&[f64::NAN],&[0.0],1).is_err());
        assert!(forward(&[1.0],&[1.0],&[f64::INFINITY],1).is_err());
        assert!(forward(&[1e308],&[1e308],&[0.0],1).is_err());
    }
}
