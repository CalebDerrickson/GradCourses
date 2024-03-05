function x_c = FindCauchyPoint(x, l, u, g, c, G)

n = length(x);

% Calculate x(t_j)
calculate_x_t_j = @(j, x, t_break, t_bar, g) arrayfun(@(k) ...
    x(k) - (t_break(j) <= t_bar(k)) * t_break(j) * g(k) ...
        - (t_break(j) > t_bar(k)) * t_bar(k) * g(k), 1:n);

%Calculate p_j
calculate_p_j = @(j, t_break, t_bar, g) arrayfun(@(k) ...
    -(t_break(j) < t_bar(k)) * g(k), 1:n);


% Calculate t_bar
t_bar = zeros(1, n);

for i =1:n
    if g(i) < 0 && u(i) < inf
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
x_j = zeros(1, n);
for j = 1:length(t_break)
    
    %Calculate p_j
    p_j = calculate_p_j(j, t_break, t_bar, g);
    
    % Calculate x(t_j)
    x_j = calculate_x_t_j(j, x, t_break, t_bar, g);
    
    % Calculate all f's
    f = dot(c, x_j) + 0.5 * dot(x_j, mtimes(G, x_j'));
    f_prime = dot(c, p_j) + dot(x_j, mtimes(G, p_j'));
    f_dprime = dot(p_j, mtimes(G, p_j'));
    
    if abs(f_dprime) < 1e-8
        dbstop in FindCauchyPoint at 61
    end

    % Finally, break into cases
    if f_prime > 0
        x_c = x_j;
        return;
    else
        % Calculate delta_t_star
        delta_t_star = -f_prime / f_dprime;
        if delta_t_star > 0 && delta_t_star < t_break(j+1) - t_break(j)
        % Return optimal value
            x_c = calculate_x_t_j(j, x, delta_t_star + t_break, t_bar, g);
            return;
        end
    end

end %for

end