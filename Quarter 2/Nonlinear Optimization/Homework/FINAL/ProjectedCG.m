function x_plus = ProjectedCG(A, G, x, gee, l, u)

% gee is g in the SolveSubproblem file. The algo in the book (16.2) has a
% different g, which I will call g in this file.

MAX_ITER = 100;

% Compute Z = null(A)
Z = null(A);

% Compute the projection
% Yikers.
P = mtimes(mtimes(Z, inv(mtimes(Z', Z))), Z');

% Compute initial r, g, d
r = gee; 
g = mtimes(P, r);
d = -g;

x_plus = x;
for i = 1:MAX_ITER
    alpha = dot(r, g) / dot(g, mtimes(G, d));
    x_plus = x_plus + alpha * d;
    r_plus = r_plus + alpha * mtimes(G, d);
    g_plus = mtimes(P, r_plus);
    beta = dot(r_plus, g_plus) / dot(r, g);
    
    %Check if not hit bound
    if (all(abs(x_plus - l) >= 1e-10)) && (all(abs(x_plus - u) >= 1e-10))
        break;
    end

    % Update
    d = -g_plus + beta * d;
    g = g_plus;
    r = r_plus;

end

end