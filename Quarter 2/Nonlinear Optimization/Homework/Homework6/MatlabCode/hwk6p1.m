function [x, obj] = hwk6p1(m, n, s, p)

rng(s);
A = randMatrix(m, n);
e = [ones(1, m) zeros(1, n - m)];
b = randSolve(A, e);
c = randObjective(A);

[x, obj] = simplex(A, b, c, e, p);

end