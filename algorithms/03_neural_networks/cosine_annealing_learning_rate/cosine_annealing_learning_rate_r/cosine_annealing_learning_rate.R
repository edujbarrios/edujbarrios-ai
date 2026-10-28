# Stateless single-period cosine learning-rate schedule.
cosine_annealing_learning_rate <- function(max_rate,min_rate,step,total_steps) {
  if (!is.numeric(max_rate) || !is.numeric(min_rate) ||
      !is.numeric(step) || !is.numeric(total_steps) ||
      is.complex(max_rate) || is.complex(min_rate) ||
      is.complex(step) || is.complex(total_steps) ||
      length(max_rate)!=1L || length(min_rate)!=1L ||
      length(step)!=1L || length(total_steps)!=1L ||
      !is.finite(max_rate) || !is.finite(min_rate) ||
      !is.finite(step) || !is.finite(total_steps) ||
      min_rate<0 || max_rate<min_rate || max_rate>1000000 ||
      total_steps<1 || total_steps>100000 ||
      total_steps!=floor(total_steps) || step!=floor(step) ||
      step<0 || step>total_steps) {
    stop("invalid finite cosine learning-rate schedule parameters")
  }
  result<-min_rate+(max_rate-min_rate)*(1+cos(pi*step/total_steps))/2
  as.double(max(min_rate,min(max_rate,result)))
}
