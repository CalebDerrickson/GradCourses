function [time, NMHM_seq] = NMHM(funcfgH, x0, eps)

%Parameters
%   funcfgH - funciton we are analyzing.
%                   Assumed to have continuous gradient and Hessian.
%   x0 - initial step. Should be 1 x 2 i.e. an array of size 2.
%   niter - number of iterations
%   n - Value for Rosenbrock order function

MAX_STEP = 500;


func = @(x, varargin) funcfgH(x, varargin{:});
global lin_systems_counter


% Initialize sequence with zeros
NMHM_seq = zeros(MAX_STEP, max(size(x0)));

% Assigning initial step
NMHM_seq(1, :) = x0;

max_size = 1;

% Main Loop
for i = 1:MAX_STEP
    [~, g, H] = func(NMHM_seq(i, :), max(size(x0)), 'dense', 'gH');

    Chol = ModifiedCholesky(H);

    % Solving Linear System with Cholesky:
    % Ax = b => Ly = b (Backward), L^T x = y (Forward)
    % A = LL^T
    
    % time it
    tic
    y = fixed.backwardSubstitute(Chol, -g');
    p_k = fixed.forwardSubstitute(Chol', y);

    time = toc;
    lin_systems_counter = lin_systems_counter + 1;

    % Getting step size from backtracking.
    % Satisfies Armijo, Curvature
    alpha = Backtracking(funcfgH, NMHM_seq(i, :), p_k);
    
     % Updating sequence
    NMHM_seq(i+1, :) = NMHM_seq(i, :) + alpha * p_k';

    %Used to resize the matrix
    max_size = i;

    % Quick Stopping Criterion
    if norm(g) < eps
        break
    end
end

if max_size ~= 1
    % Only want to return nonzero entries
    NMHM_seq = NMHM_seq(1:max_size, :);
end

end