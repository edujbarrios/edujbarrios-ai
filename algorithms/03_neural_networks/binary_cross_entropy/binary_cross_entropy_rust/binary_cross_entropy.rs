pub fn binary_cross_entropy(y:&[f64],p:&[f64])->Result<f64,&'static str>{
 if y.is_empty()||y.len()!=p.len(){return Err("invalid length")}
 let mut total=0.0_f64;
 for (&label,&prob) in y.iter().zip(p){
  if !label.is_finite()||!prob.is_finite()||(label!=0.0&&label!=1.0)||!(0.0..=1.0).contains(&prob){return Err("invalid input")}
  let term=if label==1.0 {-prob.ln()}else{-(1.0-prob).ln()};
  if !term.is_finite(){return Err("infinite loss")}
  total+=term;
  if !total.is_finite(){return Err("overflow")}
 }
 Ok(total/y.len() as f64)
}
#[cfg(test)] mod tests{
 use super::binary_cross_entropy as b;
 #[test] fn valid(){assert_eq!(b(&[1.,0.],&[1.,0.]),Ok(0.));assert!((b(&[1.,0.],&[.5,.5]).unwrap()-2f64.ln()).abs()<1e-12);}
 #[test] fn invalid(){assert!(b(&[],&[]).is_err());assert!(b(&[1.],&[0.]).is_err());assert!(b(&[2.],&[.5]).is_err());}
}
