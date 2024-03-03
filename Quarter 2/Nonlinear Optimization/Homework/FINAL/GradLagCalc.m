function grad_lag = GradLagCalc(x, c, Q_0, d_vecs, a_vecs, mu, lm, gamma)

% The last d_vec (m+1, :) is reserved for D_0!
% Separate into terms; 4 in total.
% grad_lag should be returned as a row vector.

[m, ~] = size(a_vecs);

term1 = c;
term2 = 2*mtimes(Q_0, x);
term3 = 2*gamma*mtimes( diag( d_vecs(m+1, :) ) , x);

term4 = 0;
for i = 1:m
    temp1 = (mu - lm(i)) * (dot(a_vecs(i, :), x) + gamma*dot(x, mtimes(diag(d_vecs(i, :)), x)));
    
    temp2 = temp1*(a_vecs(i, :) + 2*gamma*mtimes(diag(i, :), x));
    term4 = term4 + temp2;
end

grad_lag = term1 + term2 + term3 + term4;
grad_lag = grad_lag';

end