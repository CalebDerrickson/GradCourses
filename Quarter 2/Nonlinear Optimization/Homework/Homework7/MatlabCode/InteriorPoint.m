function [x, obj] = InteriorPoint(A, b, c, gamma, sig_min, sig_max, x0, lm0, s0, p)

%Sanitization
if iscolumn(b) b = b'; end
if iscolumn(c) c = c'; end
if iscolumn(x0) x0 = x0'; end
if iscolumn(lm0) lm0 = lm0'; end
if iscolumn(s0) s0 = s0'; end


[m, n] = size(A);

ep = 1e-5;

s_seq = zeros(p, length(s0));
s_seq(1, :) =s0;


x_seq = zeros(p, length(x0));
x_seq(1, :) =x0;

lm_seq = zeros(p, length(lm0));
lm_seq(1, :) =lm0;

obj_seq = zeros(1, p);
obj_seq(1, :) = dot(x_seq(1, :), c);

[m, n] = size(A);

e = ones(1, n);

% Create System, size is as follows, assuming A is m by n
%  n x n    n x m    n x n
%  m x n   m x m   m x n
%  n x n    n x m    n x n
%

Sys_k = [
        zeros(n)    A'                    eye(n);
        A               zeros(m)         zeros(m, n);
        diag(s_seq(1, :))  zeros(n, m)     diag(x_seq(1, :))
];

r_b = (mtimes(A, x_seq(1, :)')' - b)';
r_c = mtimes(A', lm_seq(1, :)') + s_seq(1, :)' - c';
XSe = mtimes(mtimes(diag(x_seq(1, :)), diag(s_seq(1, :))), e');

val =  sig_min * (dot(x_seq(1, :), s_seq(1, :))/n) * e';
val = val + XSe;

%Form the solution for system
sol = vertcat(-r_c(:), -r_b(:), val(:));



% For loop
num_iterations = 1;
for i = 1:p
% Choose sig_k
%Setting sigma (random for now)
sigma = (sig_max - sig_min) * rand() + sig_min;

% Solve the large system for deltas
delta = Sys_k \ sol;

%Extract the deltas
dx = delta(1:n);
dlm = delta(n+1:n + m);
ds = delta(n + m + 1:end);


% Choose largest alpha_k which stays in the set
alpha = FindAlpha(x_seq(i, :), dx, s_seq(i, :), ds, gamma, sigma);

% Set next point
x_seq(i+1, :) = x_seq(i, :) + alpha * dx';
lm_seq(i+1, :) = lm_seq(i, :) + alpha * dlm';
s_seq(i+1, :) = s_seq(i, :) + alpha * ds';
obj_seq(i+1, :) = dot(x_seq(i+1, :), c);
num_iterations = num_iterations + 1;

%Quick stopping criterion
if abs(obj_seq(i, :) - obj_seq(i+1, :)) < ep
    break;
end

% Iterate
r_b = (mtimes(A, x_seq(i+1, :)')' - b)';
r_c = mtimes(A', lm_seq(i+1, :)') + s_seq(i+1, :)' - c';
XSe = mtimes(mtimes(diag(x_seq(i+1, :)), diag(s_seq(i+1, :))), e');

val =  sigma * (dot(x_seq(i+1, :), s_seq(i+1, :))/n) * e';
val = val - XSe;

%Form the solution for system
sol = vertcat(-r_c(:), -r_b(:), val(:));

Sys_k(n+m+1:end, 1:n) = diag(s_seq(i+1, :));
Sys_k(n+m+1:end, n+m+1:end) = diag(x_seq(i+1, :));
end
% End For

x = [x_seq(num_iterations, :) lm_seq(num_iterations, :) s_seq(num_iterations, :)];
obj = dot(c, x_seq(num_iterations, :));
end