function simplex(A, b, c)
clc

%Sanitization
if iscolumn(b) b = b'; end
if iscolumn(c) c = c'; end
[m, n] = size(A);
if max(size(b)) ~= m
    fprintf("Error in sizes: expected size of b: %d. got: %d", n, max(size(b)))
    return;
end
if max(size(c)) ~= n
    fprintf("Error in sizes: expected size of c: %d. got: %d", m, max(size(c)))
    return;
end



% Create Tableau

Tab = zeros(m+1, n+1);
Tab(2:end, 1:end-1) = A;
Tab(1, 1:end - 1) = -1.*c;
Tab(2:end, end) = b';

fprintf("Tableau: \n")
disp(Tab);
fprintf("\n");

% Indicate the indices associated to N and B
temp = zeros(1, n+1);
for i = 1:n
    temp(i) = i;
end
N = temp(1:length(c));
B = temp(length(c):n);
display(N);
display(B);

% Compute x_B, lambda
% Compute s_N
% Find the first negative element of s_N (if there isn't, stop)
%   -> set q = associated index in N
% Compute d
% Calculate x_B+
% Update x_B, x_N
% Update B and N






end