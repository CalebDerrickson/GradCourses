function [x, obj] = simplex(A, b, c, e, maxloops)
clc

%Sanitization
if iscolumn(b) b = b'; end
if iscolumn(c) c = c'; end
[m, n] = size(A);
if max(size(b)) ~= m
    fprintf("Error in sizes: expected size of b: %d. got: %d", m, max(size(b)))
    x = 0;
    obj = 0;
    return;
end
if max(size(c)) ~= n
    fprintf("Error in sizes: expected size of c: %d. got: %d", m, max(size(c)))
    x = 0;
    obj = 0;
    return;
end


% Indicate the indices associated to N and B
Basis = find(e);
Nasis = uint8(find(~e));

B = A(:, Basis);
N = A(:, Nasis);

x = zeros(1, n);
x_N = zeros(1, length(Nasis));
x_B = zeros(1, length(Basis)); 

numloops = 0;
while numloops < maxloops

    % Compute x_B, lambda
    x_B = (B\b')';
    x_N = 0.*x_N;
    
    lambda = B'\c(Basis)';
    % Compute s_N
    s_N = c(Nasis)' - mtimes(N', lambda);
    
    % Find the first negative element of s_N (if there isn't, stop)
    %   -> set q = associated index in N
    % q is the entering index to Basis
    
    [min_ele, q] = min(s_N);

    if min_ele  >= 0
        fprintf("Minima found\n");
        x(Basis) = x_B;
        x(Nasis) = x_N; 
        obj = dot(c, x);
        return;
    end
    
    % Check to see if minum is degenerate.
    min_indices = (s_N == min_ele );
    if sum(min_indices) > 1
        fprintf("Degernate minimum, return\n");
        if numloops == 0 
            x(Basis) = x_B;
            x(Nasis) = x_N; 
            obj = dot(c, x); 
        end
        return;
    end

    % Compute d, 
    % Check if all d <= 0
    % note all d > 0 for next step
    
    d = B\A(:, Nasis(q));
    qual_d = [];
  
    if all (d <= 0) 
        fprintf("Problem is unbounded\n");
        if numloops == 0 
            x(Nasis) = x_N; 
            x(Basis) = x_B;
            obj = dot(c, x); 
        end
        return;
    end
    
    for i = 1:length(d)
        if (d(i) > 0)
            qual_d = [qual_d i]; %#ok<AGROW>
        end
    end
    
    % Calculate x_q+
    x_qual = zeros(1, length(qual_d));
    for i = 1:length(qual_d)
        x_qual(i) = x_B(i) / d(i);
    end
    
    [x_q, p] = min(x_qual);
    
    % Update x_B, x_N
    x_B = x_B - x_q*d';
    x_N(q) = x_q;

    %Update x_B and x_N
    x(Basis) = x_B';
    x(Nasis) = x_N;

    % Update B and N
    tempBal = Basis(p);
    Basis(p) = q;
    Nasis(q) = tempBal;
        
    B = A(:, Basis);
    N = A(:, Nasis);

numloops = numloops + 1;
end

x = [x_B x_N];
obj = dot(c, x);
end
