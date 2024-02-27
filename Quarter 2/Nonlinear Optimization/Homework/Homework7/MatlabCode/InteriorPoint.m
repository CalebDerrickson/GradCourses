function [x, obj] = InteriorPoint(A, b, c, gamma, sig_min, sig_max, x0)

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
MAX_ITER = 10;

IntPoint_Seq = zeros(MAX_ITER, length(x0));
IntPoint_Seq(1, :) = x0;


Sys = zeros()

% For loop
for i = 1:MAX_ITER
% Choose sig_k
sigma = (sig_max - sig_min) * rand() + sig_min;

% Solve the large system for deltas


% Choose largest alpha_k which stays in the set
% Iterate
end
% End For


end