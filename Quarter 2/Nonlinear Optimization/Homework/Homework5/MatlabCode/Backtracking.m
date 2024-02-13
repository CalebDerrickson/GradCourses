function alpha = Backtracking(funcfgH, x, p_k )
global func_counter


% Parameters
%   func_eval - pointer to function we are evaluating
%   x - current iterate for checking step
%   n - Only used for Rosenbrock function
    
%Constants for the backtracking
rho = 0.5;
c1 = 1e-4;
alpha = 1;

func = @(x, varargin) funcfgH(x, varargin{:});

%Sanitization
if iscolumn(p_k) p_k = p_k'; end


%Priming for while loop
[f_k, grad_k, ~] = func(x, max(size(x)), '', 'fg');
x_next = x + alpha * p_k;
target = f_k + c1 * alpha * dot(grad_k, p_k);
[current, ~, ~] = func(x_next, max(size(x_next)), '', 'f');

func_counter = func_counter + 2;

while current > target
 
    alpha = alpha * rho;

    x_next = x + alpha * p_k;
    target = f_k + c1 * alpha * dot(grad_k,p_k);
    [current, ~, ~] = func(x_next, max(size(x_next)), '', 'f');

    func_counter = func_counter + 2;
end

end