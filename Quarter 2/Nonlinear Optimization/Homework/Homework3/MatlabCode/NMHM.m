function NMHM_seq = NMHM(funcfgH, x0, niter, n, eps)

%Parameters
%   funcfgH - funciton we are analyzing.
%                   Assumed to have continuous gradient and Hessian.
%   x0 - initial step. Should be 1 x 2 i.e. an array of size 2.
%   niter - number of iterations
%   n - Value for Rosenbrock order function

% Initialize sequence with zeros
NMHM_seq = zeros(niter, 2);

% Assigning initial step
NMHM_seq(1, :) = x0;

% Main Loop
for i = 1:niter - 1
    [~, g, H] = funcfgH(NMHM_seq(i, :), n);

    Mod_cholesky = ModifiedCholesky(H);

    % Solving Linear System with Cholesky:
    % Ax = b => Ly = b (forward), L^T x = y (backward)
    % A = LL^T
    
    y = fixed.forwardSubstitute(Mod_cholesky, -g');
    p_k = fixed.backwardSubstitute(Mod_cholesky', y);

    % Getting step size from backtracking.
    % Satisfies Armijo, Curvature
    alpha = Backtracking(funcfgH, NMHM_seq(i, :), n);

     % Updating sequence
    NMHM_seq(i+1, :) = NMHM_seq(i, :) + alpha * p_k';

    %Used to resize the matrix
    max_size = i;

    % Quick Stopping Criterion
    if norm(g) < eps
        break
    end
end

% Only want to return nonzero entries
NMHM_seq = NMHM_seq(1:max_size, :);
end