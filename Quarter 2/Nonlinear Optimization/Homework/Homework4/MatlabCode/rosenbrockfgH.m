function [f, g, H] = rosenbrockfgH(x, n, HessianType, evals)

% Function to evaluate the order n RosenbrockrosenbrockfgH function,
%   its gradient, and its hessian at the given x. 

% Parameters 
%   x - Evaluation point
%   n - Order of summation.
%       Should also coincide with size of x.
%   HessianType - Storage type of the Hessian.
%       Basically either sparse or dense
%   evals - what you want to have evaluated.
%       Saves computation time if you just want a subset.


arguments
    x
    n = max(size(x));
    HessianType = 'dense'
    evals = 'everything'
end

FUNCTION_FLAG = 1;
GRADIENT_FLAG = 2;
HESSIAN_FLAG = 4;


H = zeros(n, n);
g = zeros(1, n);
f = 0;


switch evals
    case 'f'
        eval_code = FUNCTION_FLAG;
    case 'g'
        eval_code = GRADIENT_FLAG;
    case 'H'
        eval_code = HESSIAN_FLAG;
    case 'fg'
        eval_code = bitor(FUNCTION_FLAG, GRADIENT_FLAG);
    case 'fH'
        eval_code = bitor(FUNCTION_FLAG, HESSIAN_FLAG);
    case 'gH'
        eval_code = bitor(GRADIENT_FLAG, HESSIAN_FLAG);
    otherwise
        eval_code = bitor(FUNCTION_FLAG, GRADIENT_FLAG);
        eval_code = bitor(eval_code, HESSIAN_FLAG);
end

if bitand(eval_code, FUNCTION_FLAG)
    func = @(x, n) sum( 100* (x(2:end) - x(1:end-1).^2).^2 + (x(1:end-1) - 1).^2);
    f = func(x, n);
end

if bitand(eval_code, GRADIENT_FLAG)
    % Construct the gradient
    for i = 1:n
        if i == 1
            g(i) = -400 * x(1) * (x(2) - x(1)^2) + 2*(x(1) - 1);
        elseif i == n
            g(i) = 200*(x(i) - x(i-1)^2);
        else
            g(i) = 200 * (x(i) - x(i-1)^2) - 400 * x(i) * (x(i+1) - x(i)^2) + 2*(x(i) - 1);
        end
    end
end

if bitand(eval_code, HESSIAN_FLAG)
    % Construct the Hessian
    for i = 2:n-1
        for j = 2:n-1
            if j == i
                H(i, j) = 202 + 1200 * x(i)^2 - 400 * x(i+1);
            elseif j == i+1
                H(i, j) = -400 * x(i);
            elseif j == i-1
                H(i, j) = -400 * x(i-1);
            end
        end
    end
    
    % Special cases
    H(1, 1) = 1200 * x(1)^2 - 400 * x(2) + 2;
    H(1, 2) = -400 * x(1);
    H(2, 1) = H(1, 2);
    H(end, end) = 200;
    H(end, end-1) = -400 * x(end-1);
    H(end-1, end) = H(end, end-1);
    
    switch HessianType
        case 'sparse'
            H = sparse(H);
        case 'dense'
            % Empty
        otherwise
            fprintf("Invalid parameter for Hessian storage type, returning as dense")
    end
end

end