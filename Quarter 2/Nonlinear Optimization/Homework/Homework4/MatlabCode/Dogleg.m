function Dogleg_search = Dogleg(function_handle, x, delta)

func = @(x, varargin) func_handle(x, varargin{:});

% Funciton evaluation, gradient, and sparse Hessian
[f, g, H] = func(x, max(size(x)), 'sparse', 'everything');
% Change g to a column vector
g = g';

% Get the Cholesky factorization of H
Chol = ModifiedCholesky(H);

% Solve for p_B
% Solving Linear System with Cholesky:
% Ax = b => Ly = b (Backward), L^T x = y (Forward)
% A = LL^T

y = fixed.backwardSubstitute(Chol, -g);
p_B = fixed.forwardSubstitute(Chol', y);

% Is newton step sufficient
if norm(p_B) <= delta
    Dogleg_search = p_B;
else % Preform the Stanky Leg
    p_U = -(dot(g, g)/dot(g, mtimes(H, g))) * g;
    if norm(p_U) >= delta
        % Use steepest descent
        Dogleg_search = (delta/norm(p_U))*p_U;
    else
        tau = QuadSearch(p_B, p_U, delta);
        if abs(tau) <= 1
            Dogleg_search = tau * p_U;
        elseif  (1 <tau) && (tau <= 2)
            Dogleg_search = p_U + (tau - 1)*(p_B - p_U);
    end

end
end