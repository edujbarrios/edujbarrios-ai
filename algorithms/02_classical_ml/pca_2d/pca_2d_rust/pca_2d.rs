//! The dominant principal component of n row-major two-feature samples.

/// Return (mean, unit direction, projected values), reducing 2D data to 1D.
pub fn pca_2d(points: &[f64]) -> Result<([f64;2],[f64;2],Vec<f64>),&'static str> {
    if points.len()<4 || points.len()%2!=0 {
        return Err("at least two two-feature observations required");
    }
    let n=points.len()/2;
    let (mut mx,mut my)=(0.0_f64,0.0_f64);
    for (i,row) in points.chunks_exact(2).enumerate() {
        if !row[0].is_finite() || !row[1].is_finite() {
            return Err("non-finite observation");
        }
        let dx=row[0]-mx;
        let dy=row[1]-my;
        if !dx.is_finite() || !dy.is_finite() {return Err("mean overflow");}
        mx+=dx/(i+1) as f64;my+=dy/(i+1) as f64;
        if !mx.is_finite() || !my.is_finite() {return Err("mean overflow");}
    }
    let (mut a,mut b,mut c)=(0.0_f64,0.0_f64,0.0_f64);
    for row in points.chunks_exact(2) {
        let dx=row[0]-mx;
        let dy=row[1]-my;
        let (xx,xy,yy)=(dx*dx,dx*dy,dy*dy);
        if !xx.is_finite() || !xy.is_finite() || !yy.is_finite() {
            return Err("covariance overflow");
        }
        a+=xx;b+=xy;c+=yy;
        if !a.is_finite() || !b.is_finite() || !c.is_finite() {
            return Err("covariance overflow");
        }
    }
    a/=n as f64;b/=n as f64;c/=n as f64;
    if a==0.0 && c==0.0 {return Err("zero covariance");}
    let (mut vx,mut vy);
    if b==0.0 {
        (vx,vy)=if a>=c {(1.0,0.0)}else{(0.0,1.0)};
    } else {
        let delta=a-c;
        let twice_b=2.0*b;
        let disc=delta.hypot(twice_b);
        vx=delta+disc;vy=twice_b;
        let length=vx.hypot(vy);
        if !delta.is_finite() || !twice_b.is_finite() ||
           !disc.is_finite() || !vx.is_finite() ||
           !length.is_finite() || length<=0.0 {
            return Err("eigenvector overflow");
        }
        vx/=length;vy/=length;
        if vx<0.0 || (vx==0.0 && vy<0.0) {vx=-vx;vy=-vy;}
    }
    let mut projected=Vec::with_capacity(n);
    for row in points.chunks_exact(2) {
        let dx=row[0]-mx;let dy=row[1]-my;
        let px=dx*vx;let py=dy*vy;let value=px+py;
        if !dx.is_finite() || !dy.is_finite() ||
           !px.is_finite() || !py.is_finite() || !value.is_finite() {
            return Err("projection overflow");
        }
        projected.push(value);
    }
    Ok(([mx,my],[vx,vy],projected))
}

#[cfg(test)]
mod tests {
    use super::pca_2d;
    #[test]
    fn axis_examples() {
        let (mean,v,p)=pca_2d(&[0.0,0.0,2.0,0.0,4.0,0.0]).unwrap();
        assert_eq!(mean,[2.0,0.0]);assert_eq!(v,[1.0,0.0]);
        assert_eq!(p,vec![-2.0,0.0,2.0]);
        let (mean,v,p)=pca_2d(&[0.0,0.0,0.0,2.0]).unwrap();
        assert_eq!(mean,[0.0,1.0]);assert_eq!(v,[0.0,1.0]);
        assert_eq!(p,vec![-1.0,1.0]);
    }
    #[test]
    fn diagonal_and_ties() {
        let (_,v,_)=pca_2d(&[0.0,0.0,2.0,2.0,4.0,4.0]).unwrap();
        assert!((v[0]-0.5_f64.sqrt()).abs()<1e-12);
        assert!((v[1]-0.5_f64.sqrt()).abs()<1e-12);
        let (_,v,_)=pca_2d(&[1.0,0.0,-1.0,0.0,0.0,1.0,0.0,-1.0]).unwrap();
        assert_eq!(v,[1.0,0.0]);
    }
    #[test]
    fn invalid() {
        assert!(pca_2d(&[]).is_err());
        assert!(pca_2d(&[0.0,0.0]).is_err());
        assert!(pca_2d(&[1.0,1.0,1.0,1.0]).is_err());
        assert!(pca_2d(&[1.0,2.0,3.0]).is_err());
        assert!(pca_2d(&[0.0,f64::NAN,2.0,0.0]).is_err());
        assert!(pca_2d(&[-1e308,0.0,1e308,0.0]).is_err());
    }
}
