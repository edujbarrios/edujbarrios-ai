//! Type-7 sample quartiles and their interquartile range.

fn quantile(sorted:&[f64],p:f64)->f64{
    let h=(sorted.len()-1) as f64 * p;
    let i=h as usize;
    let fraction=h-i as f64;
    if fraction==0.0 { sorted[i] }
    else { (1.0-fraction)*sorted[i]+fraction*sorted[i+1] }
}

/// Return Q3-Q1 (type 7), rejecting invalid inputs and overflow.
pub fn interquartile_range(values:&[f64])->Result<f64,&'static str>{
    if !(1..=512).contains(&values.len()) || values.iter().any(|v|!v.is_finite()){
        return Err("expected 1..512 finite observations");
    }
    let mut sorted=values.to_vec();
    sorted.sort_by(f64::total_cmp);
    let result=quantile(&sorted,0.75)-quantile(&sorted,0.25);
    if !result.is_finite(){return Err("nonfinite interquartile range");}
    Ok(result.max(0.0))
}

#[cfg(test)]
mod tests{
    use super::interquartile_range as iqr;
    fn near(v:&[f64],want:f64){
        let actual=iqr(v).unwrap();
        assert!((actual-want).abs()<=1e-11f64*want.abs().max(1.0));
    }
    #[test]
    fn examples(){
        near(&[1.,2.,3.,4.],1.5);
        near(&[1.,2.,3.,4.,100.],2.);
        near(&[1.,5.],2.);
        near(&[1.,2.,3.],1.);
        near(&[42.],0.);
        near(&[7.;512],0.);
        near(&[100.,4.,3.,2.,1.],2.);
        near(&[-5.,-3.,-1.,1.,3.],4.);
        near(&[1.,1.,1.,50.,50.,50.],49.);
        near(&[-1e308,1e308],1e308);
    }
    #[test]
    fn invalid(){
        assert!(iqr(&[]).is_err());
        assert!(iqr(&[1.;513]).is_err());
        assert!(iqr(&[f64::INFINITY]).is_err());
        assert!(iqr(&[f64::NAN]).is_err());
        assert!(iqr(&[-1e308,-1e308,1e308,1e308]).is_err());
    }
}
