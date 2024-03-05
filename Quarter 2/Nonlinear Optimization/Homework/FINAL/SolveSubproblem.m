function x_s = SolveSubproblem(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, l, u)

if isrow(x)
    x = x';
end

% Given a feasible starting point x0
x_s = x;

% For k = 1, ...
MAX_ITER = 100;
for k = 1:MAX_ITER
    if isrow(x_s)
        x_s = x_s';
    end

    % Calculate g = Gx + c
    [c, G]  = CalculateG(x_s, see, Q_0, d_vecs, a_vecs, mu, lm, gamma);
    g = mtimes(G, x_s) + c';


    
    % set x = x_k and find the Cauchy point x^c
    x_k = x_s;
    x_c = FindCauchyPoint(x_k, l, u, g, c, G);

    % Find an approximate solution x_+ of 16.74 st q(x_+) leq q(x^c) (Use Algo
    %       16.1 or 16.2) and x_+ is feasible
    % set x_k+1 = x_+

    % Find points of x_k which correspond to the Cauchy point (if any)
    % Arbitrary tolerance
    % Wait just have cauchy point be next iteration for now
    %xc_eq = abs(x_k - x_c) < 1e-10; 
    %A = diag(xc_eq);
    
    %x_kk = ProjectedCG(A, G, x_k, g, l, u);
    %x_s = x_kk;
    x_s = x_c;

    if all(all(l <= x_s)) && all(all(x_s <= u))
        % stop with x_s being the solution
        return;
    end
    
end
% end(for)
end