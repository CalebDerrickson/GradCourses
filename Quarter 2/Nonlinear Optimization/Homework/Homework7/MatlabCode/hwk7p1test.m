function hwk7p1p2()
clc
m = 50;
n = 2*m;
s = 20;

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

[~ ,~] = InteriorPoint(A, b, c, gamma, 0.1, 0.9, x0, lm0, s0);

end