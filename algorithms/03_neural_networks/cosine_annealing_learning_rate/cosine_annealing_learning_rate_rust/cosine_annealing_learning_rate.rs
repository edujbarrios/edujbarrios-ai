//! Stateless cosine learning-rate annealing for one period.

/// Evaluate min + 0.5*(max-min)*(1+cos(pi*step/total_steps)).
pub fn cosine_annealing_learning_rate(
    max_rate: f64, min_rate: f64, step: usize, total_steps: usize
) -> Result<f64, &'static str> {
    if !max_rate.is_finite() || !min_rate.is_finite() ||
       min_rate<0.0 || max_rate<min_rate || max_rate>1_000_000.0 ||
       !(1..=100_000).contains(&total_steps) || step>total_steps {
        return Err("invalid cosine annealing parameters");
    }
    let ratio=step as f64/total_steps as f64;
    let fraction=(1.0+(std::f64::consts::PI*ratio).cos())*0.5;
    let result=min_rate+(max_rate-min_rate)*fraction;
    if !result.is_finite() {return Err("nonfinite rate");}
    Ok(result.clamp(min_rate,max_rate))
}
#[cfg(test)]
mod tests {
    use super::cosine_annealing_learning_rate as rate;
    fn close(a:f64,b:f64) {assert!((a-b).abs()<1e-12,"{a} != {b}");}
    #[test] fn schedule() {
        close(rate(1.,0.,0,4).unwrap(),1.);
        close(rate(1.,0.,4,4).unwrap(),0.);
        close(rate(1.,0.,2,4).unwrap(),0.5);
        close(rate(1.,0.,1,4).unwrap(),0.8535533905932737);
        close(rate(1.,0.,3,4).unwrap(),0.14644660940672627);
        assert_eq!(rate(0.5,0.5,1,3),Ok(0.5));
        assert_eq!(rate(1e6,0.,1,1),Ok(0.));
    }
    #[test] fn invalid() {
        assert!(rate(0.,1.,1,4).is_err());
        assert!(rate(1.,-1.,1,4).is_err());
        assert!(rate(1_000_001.,0.,1,4).is_err());
        assert!(rate(f64::NAN,0.,1,4).is_err());
        assert!(rate(f64::INFINITY,0.,1,4).is_err());
        assert!(rate(1.,0.,1,0).is_err());
        assert!(rate(1.,0.,5,4).is_err());
        assert!(rate(1.,0.,1,100001).is_err());
    }
}
