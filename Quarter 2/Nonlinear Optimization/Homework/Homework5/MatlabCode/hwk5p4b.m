function hwk5p4b(x0, eps, niter)

clc
BFGS_seq = BFGS(x0, eps, @rosenbrockfgH, niter);


n = max(size(x0));
xstar = ones(n, 1);
Norm_seq = zeros(n, 1);

for i = 1:max(size(BFGS_seq))
    Norm_seq(i) = norm(xstar- BFGS_seq(i, :));
end


semilogy(Norm_seq, LineWidth=2);
grid on;
title("Superlinear Convergence for n=30");
ylabel("$|| x_k - x^* ||$", Interpreter="latex");
xlabel("Iterate $k$", Interpreter="latex");

end