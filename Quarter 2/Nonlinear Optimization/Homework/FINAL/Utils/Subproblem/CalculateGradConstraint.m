function A = CalculateGradConstraint(x, a_vecs, d_vecs, gamma)

[m, n] = size(a_vecs);
A = zeros(m, n);

for i = 1:m
    temp = a_vecs(i, :) + 2*gamma*mtimes(diag(d_vecs(i, :)), x);
    A(i, :) = temp;
end


end