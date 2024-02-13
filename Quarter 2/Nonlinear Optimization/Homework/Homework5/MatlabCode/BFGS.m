function BFGS_seq = BFGS(x0, epsilon, funcfgH, niter, H0)

arguments
    x0
    epsilon
    funcfgH
    niter = 100;
    H0 = eye(max(size(x0)), max(size(x0)));
end



%Sanitization
if iscolumn(x0) x0 = x0'; end

n = max(size(x0));
func = @(x, varargin) funcfgH(x, varargin{:});


BFGS_seq = zeros(niter, n);
grad_seq = zeros(niter, n);

%Initial starting
[~, grad_seq(1, :) , ~] = func(x0, n, '', 'g');
ngrad = norm(grad_seq(1, :));
BFGS_seq(1, :) = x0;


k = 1;
B = H0;

while (ngrad > epsilon) && (k <= niter)

    p_k = -(B\(grad_seq(k, :)'))';

    
    alpha = Backtracking(funcfgH, BFGS_seq(k, :), p_k);

    BFGS_seq(k+1, :) = BFGS_seq(k, :) + alpha * p_k;
    [~, grad_seq(k+1, :), ~] = func(BFGS_seq(k+1, :), n, '', 'g');
    
    ngrad = norm(grad_seq(k+1, :));

    s_k = BFGS_seq(k+1, :) - BFGS_seq(k, :);
    y_k = grad_seq(k+1, :) - grad_seq(k, :);

    B = HessianUpdate(B, s_k, y_k);
    k = k+1;
end

if k ~= niter
    BFGS_seq = BFGS_seq(1:k, :);
end

end