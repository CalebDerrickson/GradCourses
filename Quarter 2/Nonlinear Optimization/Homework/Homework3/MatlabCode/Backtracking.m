function alpha = Backtracking(func_eval, x, p_k , n)

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
x_next = x + alpha * p_k';
target = f_k + c1 * alpha * grad_k * p_k;
[current, ~, ~] = func_eval(x_next, n);


while current > target
 
    alpha = alpha * rho;

    x_next = x + alpha * p_k';
    target = f_k + c1 * alpha * grad_k * p_k;
    [current, ~, ~] = func_eval(x_next, n);
end

end