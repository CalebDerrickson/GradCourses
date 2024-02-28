function [x, obj] = hwk7p1(m, n, s, p)

clc
rng(s);
A = randMatrix(m, n);
e = ones(1, n);
b = randSolve(A, e);
[c, u] = randObjective(A);

x0 = e;
lm0 = mtimes(inv(mtimes(A, A')), b);
s0 = 100.*u;

u_avg = mean(u);
gamma = min((1/u_avg).*u);

[x ,obj] = InteriorPoint(A, b, c, gamma, 0.1, 0.9, x0, lm0, s0, p);
end