function [c, q_0, d_vecs, a_vecs] = GetRandomVariables(m,n)

% Form c
c = zeros(1, n);
for i = 1:n    
    c(i) = rand();
end

% Form q_0
q_0 = zeros(1, n);
for i = 1:n
    q_0(i) = 0.5 + rand();
end

% Form d_vecs and a_vecs
% Need another d for D_0
% Test this.
d_vecs = zeros(m+1, n);
a_vecs = zeros(m, n);

for j = 1:n
    for i = 1:m
    d_vecs(i, j) = -1 + 2*rand();
    a_vecs(i, j) = rand();
    end
end

for i = 1:n
    d_vecs(end, i) = -1+2*rand();
end

end