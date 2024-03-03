function x_proj = Projection(g, l, u)

% l and u should be passed as vectors

n = length(g);
x_proj = zeros(1, n);

for i = 1:n
    if g(i) <= l(i)
        x_proj(i) = l(i);

    elseif g(i) >= u(i)
        x_proj(i) = u(i);

    else
        x_proj(i) = g(i);
    end
end

end