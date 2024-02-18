function mat = randMatrix(m, n)

mat = zeros(m, n);
randomvals = randn(m * n, 1);

for i = 1:m
    for j = 1:n
        mat(i, j) = randomvals(j + i * n);
    end
end


end