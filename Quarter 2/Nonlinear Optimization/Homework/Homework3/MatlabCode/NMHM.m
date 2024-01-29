function NMHM_seq = NMHM(funcfgH, x0, niter, n, eps)

%Parameters
%   funcfgH - funciton we are analyzing.
%                   Assumed to have continuous gradient and Hessian.
%   x0 - initial step. Should be 1 x 2 i.e. an array of size 2.
%   niter - number of iterations
%   n - Value for Rosenbrock order function

sz = size(x0);
% Initialize sequence with zeros
NMHM_seq = zeros(niter, sz(2));

%Norm gradient seq
NG_seq = zeros(niter, 1);
alpha_seq = zeros(niter, 1);

% Assigning initial step
NMHM_seq(1, :) = x0;

max_size = 1;

% Main Loop
for i = 1:niter - 1
    [~, g, H] = funcfgH(NMHM_seq(i, :), n);

    Chol = ModifiedCholesky(H);

    % Solving Linear System with Cholesky:
    % Ax = b => Ly = b (Backward), L^T x = y (Forward)
    % A = LL^T
    
    y = fixed.backwardSubstitute(Chol, -g');
    p_k = fixed.forwardSubstitute(Chol', y);

    % Getting step size from backtracking.
    % Satisfies Armijo, Curvature
    alpha = Backtracking(funcfgH, NMHM_seq(i, :), p_k, n);
    
     % Updating sequence
    NMHM_seq(i+1, :) = NMHM_seq(i, :) + alpha * p_k';

    %Used to resize the matrix
    max_size = i;
    NG_seq(i) = norm(g);
    alpha_seq(i) = alpha;

    % Quick Stopping Criterion
    if norm(g) < eps
        break
    end
end

%TEMP
figure(1)
semilogy(NG_seq)
title("NMHM Grad seq")
figure(3)
semilogy(alpha_seq)
title("Alpha")


if max_size ~= 1
    % Only want to return nonzero entries
    NMHM_seq = NMHM_seq(1:max_size, :);
end

end