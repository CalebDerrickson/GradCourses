function hwk6p2()

m = 5;
n = 10;
%rng(7)
A = randMatrix(m, n);
e = [ones(1, m) zeros(1, n - m)];
b = randSolve(A, e);
c = randObjective(A);

[x, ~] = simplex(A, b, c, e, 4);

display(x);

end