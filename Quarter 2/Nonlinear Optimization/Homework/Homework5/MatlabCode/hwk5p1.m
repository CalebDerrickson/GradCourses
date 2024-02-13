function [x] = hwk5p1(x0, eps, niter)

clc
BFGS_seq = BFGS(x0, eps, @rosenbrockfgH, niter);
[x] = BFGS_seq(end, :);

end