function [c,G] = CalculateG(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma)

% a_vecs will be accessed as ROW vectors, as well as d_vecs
% The last d_vec (m+1, :) is reserved for D_0!
% see is a stand in for c in the original problem


[m, n] = size(a_vecs);

% Calcuate G
G = zeros(n, n);
G = G + 2.*Q_0;
G = G + (2*gamma).*diag(d_vecs(m+1, :));

for i = 1:m
    temp = (lm(i) * 2 * gamma).*diag(d_vecs(i, :));
    G = G - temp;
end

% Calculate c
c = GradLagCalc(x, see, Q_0, d_vecs, a_vecs, mu, lm, gamma);



    
end