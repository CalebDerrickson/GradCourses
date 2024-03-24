function flag = IsKKTSatisfied(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma, g)

flag = false;
[m, n] = size(a_vecs);

if ~isequal(g, zeros(size(g)))
    return
end

% Get the Grad Constraints
[constr_vec, ~] = CalculateConstraint(x, a_vecs, d_vecs, gamma);

if ~isequal(constr_vec, zeros(size(constr_vec)))
    return;
end

for i = 1:m
    if ~isequal(lm(i) * constr_vec(i) , 0)
        return;
    end
end

flag = true;
end