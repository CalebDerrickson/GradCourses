function lag = LagCalc(x, c, Q_0, d_vecs, a_vecs, mu, lm, gamma)

% The last d_vec (m+1, :) is reserved for D_0!
% Separate into terms; 4 in total.
% lag should be returned as a row vector.

[m, ~] = size(a_vecs);

term1 = dot(c, x);
term2 = dot(x, mtimes(Q_0, x));
term3 = gamma * (dot(x, mtimes(diag(d_vecs(m+1, :)), x) ));

term4 = 0;
for i = 1:m
    temp = (mu / 2 - lm(i)) * (dot(a_vecs(i, :), x + gamma * dot(x, mtimes(diag(d_vecs(i, :)), x))));
    term4 = term4 + temp;
end

lag = term1 + term2 + term3 + term4;

lag = lag';

end