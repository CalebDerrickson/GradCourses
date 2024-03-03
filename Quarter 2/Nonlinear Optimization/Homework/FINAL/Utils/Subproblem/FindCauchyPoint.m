function x_c = FindCauchyPoint(x, l, u, g, c, G)

n = length(x);

% Calculate t_bar
t_bar = zeros(1, n);

for i =1:n
    if g(i) < 0 && u_i < inf
        t_bar(i) = (x(i) - u(i)) / g(i);
    
    elseif g(i) > 0 && l(i) > -inf
        t_bar(i) = (x(i) - l(i)) / g(i);

    else 
        t_bar(i) = inf;

    end
end

% Remove zero values and duplicates from t_bar
t_break = unique(t_bar);
t_break = sort(t_break);
t_break = t_break(t_break ~= 0);


p_j = zeros(1, n);
for j = 1:length(t_break)
    
    %Calculate p_j
    for k = 1:n
        if t_break(j) < t_bar(k)
            p_j(k) = -g(k);
        else
            p_j(k) = 0;
        end
    end

    % Calculate all f's
    f
    f_prime
    f_dprime

end
end