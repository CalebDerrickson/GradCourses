function alpha = Backtracking(funcfgH, x, p_k )

% Parameters
%   func_eval - pointer to function we are evaluating
%   x - current iterate for checking step
%   n - Only used for Rosenbrock function
    
%Constants for the backtracking
rho = 0.5;
c1 = 1e-3;
alpha = 1;

func = @(x, varargin) funcfgH(x, varargin{:});


%Priming for while loop
[f_k, grad_k, ~] = func(x, max(size(x)), '', 'fg');
x_next = x + alpha * p_k';
target = f_k + c1 * alpha * grad_k * p_k;
[current, ~, ~] = func(x_next, max(size(x_next)), '', 'f');


while current > target
 
    alpha = alpha * rho;

    x_next = x + alpha * p_k';
    target = f_k + c1 * alpha * grad_k * p_k;
    [current, ~, ~] = func(x_next, max(size(x_next)), '', 'f');
end

end