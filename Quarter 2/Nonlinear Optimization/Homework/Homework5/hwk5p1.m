function hwk5p1(x0, eps, niter)

BFGS_seq = BFGS(x0, eps, @rosenbrockfgH, niter);

end