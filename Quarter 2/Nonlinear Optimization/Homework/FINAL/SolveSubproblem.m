function x_s = SolveSubproblem(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, l, u)

% Given a feasible starting point x0
x_s = x;

% For k = 1, ...
MAX_ITER = 100;
for k = 1:MAX_ITER
    

    % Calculate g = Gx + c
    [c, G]  = CalculateG(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma);
    g = mtimes(G, x) + c;

    % If the KKT conditions are satisfied (for g = Gx + c)
    if IsKKTSatisfied(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, g)
        % stop with x_s being the solution
        return;
    end
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
end
% end(for)
end