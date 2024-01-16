function alpha = Backtracking(x, c)
    
%Constants for the backtracking
rho = 0.5;
c1 = 1e-4;
alpha = 1;

%Priming for while loop
[f_k, grad_k] = FunctionEvaluation(x, c);
x_next = x - alpha * grad_k;
Target = f_k - c1 * alpha * norm(grad_k);
[Current, ~] = FunctionEvaluation(x_next, c);

while  Current > Target

    alpha = alpha * rho;

    x_next = x - alpha * grad_k;
    Target = f_k - c1 * alpha * norm(grad_k);
    [Current, ~] = FunctionEvaluation(x_next, c);

end

end