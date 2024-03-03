function x_s = SolveSubproblem(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, l, u)

% Given a feasible starting point x0
x_s = x;

% For k = 1, ...
MAX_ITER = 100;
for k = 1:MAX_ITER
    % If the KKT conditions are satisfied (for g = Gx + c)

    % Calculate g = Gx + c
    [c, G]  = CalculateG(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma);
    g = mtimes(G, x) + c;

    if IsKKTSatisfied(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, g)
        % stop with x being the solution
        return;
    end
    % set x = x_k and find the Cauchy point x^c
    x_k = x;
    x_c = FindCauchyPoint(x, l, u, g, c, G);

    % Find an approximate solution x_+ of 16.74 st q(x_+) leq q(x^c) (Use Algo
    %       16.1 or 16.2) and x_+ is feasible
    % set x_k+1 = x_+
    
end
% end(for)
end