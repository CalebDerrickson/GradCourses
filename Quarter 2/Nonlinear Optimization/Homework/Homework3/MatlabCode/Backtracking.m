function alpha = Backtracking(func_eval, x, n)

% Parameters
%   func_eval - pointer to function we are evaluating
%   x - current iterate for checking step
%   n - Only used for Rosenbrock function
    
%Constants for the backtracking
rho = 0.5;
c1 = 1e-4;
alpha = 1;

%Priming for while loop
[f_k, grad_k, ~] = func_eval(x, n);
x_next = x - alpha * grad_k;
Target = f_k - c1 * alpha * norm(grad_k);
[Current, ~, ~] = func_eval(x_next, n);

while  Current > Target

    alpha = alpha * rho;

    x_next = x - alpha * grad_k;
    Target = f_k - c1 * alpha * norm(grad_k);
    [Current, ~, ~] = func_eval(x_next, n);

end

end