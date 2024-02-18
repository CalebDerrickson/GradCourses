function b = randSolve(A,m)

% m is the size of number of non zero entries in e
n = size(A(:, 1));
e = [ones(1, m) zeros(1, n)];

b = mtimes(A, e');

end