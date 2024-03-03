function [constr_vec, constr_norm] = CalculateConstraint(x, a_vecs, d_vecs, gamma)

% a_vecs will be accessed as ROW vectors, as well as d_vecs
% The last d_vec (m+1, :) is reserved for D_0!
[m, n] = size(a_vecs);

constr_vec = zeros(1, m);

for i =1:m
    temp = dot(a_vecs(i, :), x) + gamma* dot(x, mtimes( diag( d_vecs(i, :) ), x));
    constr_vec(i, :) = temp;
end

constr_norm = norm(constr_vec);

end
