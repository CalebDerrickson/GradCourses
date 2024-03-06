function flag = OPTCheck(x, Q_0, a_vecs, d_vecs, gamma, lm, mu)

flag = false;

[m, ~] = size(a_vecs);

val = 0;

val = val + 2.*(Q_0  + diag(d_vecs(m+1, :)));

for i =1:m
    val = val - 2*gamma*lm(i) * diag(d_vecs(i, :));
end

for i = 1:m
    temp1 = mu*(dot(a_vecs(i, :), x) + gamma*dot(x, mtimes(diag(d_vecs(i, :)), x')));
    temp2 = mu*(2*gamma*mtimes(diag(d_vecs(i, :)), x'));
    
    temp3 = (a_vecs(i, :)' + mtimes(diag(d_vecs(i, :)), x')).*(a_vecs(i, :)' + mtimes(diag(d_vecs(i, :)), x'))';
    scalar = (4*gamma^2*mu);
    temp3 = scalar*(temp3);
    val = val + temp1*temp2+ temp3;
end

[~, succ] = chol(val);

if ~succ
    flag = true;
end

end