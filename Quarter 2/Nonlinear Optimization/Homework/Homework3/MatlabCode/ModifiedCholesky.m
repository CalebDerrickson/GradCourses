function Chol = ModifiedCholesky(A)

% Tries to apply Cholesky to A. 
% If it fails, run Cholesky again with (A + tau * I), where
%   tau is some parameter
%   I is the identity matrix of same size as A
%   tau * I makes A "Sufficiently Positive Definite"

% Setting parameters
beta = 10e-3;
min_diag_element = min(diag(A));
Id = eye(size(A));

% Setting a max iteration bound
MAX_ITER = 1e3;

% Assigning tau as described
if min_diag_element < 0
    tau = 0;
else
    tau = -1* min_diag_element + beta;
end

% Main Loop
for k = 0:MAX_ITER
    [Chol, flag] = chol(A + tau * Id);
    if flag == 0
        return 
    else 
        tau = max(2 * tau, beta);
    end %If
end %For

% Only runs statement below when k == MAT_ITER
fprintf("Iteration could not be found for A in %d steps", MAX_ITER);

end % Function