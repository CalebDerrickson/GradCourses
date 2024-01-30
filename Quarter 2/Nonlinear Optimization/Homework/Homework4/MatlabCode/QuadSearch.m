function tau = QuadSearch(p_B, p_U, delta)
%THIS IS NOT A GENERIC QUADRATIC SEARCH.
%ONLY SHOULD BE USED FOR DOGLEG


%Cauchy point calculation
p_C = p_B - p_U;

% Coefficients of polynomial
coeffs = [norm(p_C)^2, 2*dot(p_C, p_U), norm(p_U)^2 - delta^2];
tau = max(roots(coeffs));

end