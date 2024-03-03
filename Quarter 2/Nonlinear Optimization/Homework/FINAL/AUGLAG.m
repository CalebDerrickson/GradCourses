function [x_seq, obj_seq] =  AUGLAG(c, Q, a_vecs, d_vecs, gamma)

[m, n] = size(Q);
% Choose an initial point x0 and initial multipliers lm0
% The origin is given as a feasible point. 

% Choose convergence tolerances eta_star and om_star
% Don't know what these should exactly by atm. Temp values.
eta_star = 5;
om_star = 5;

% Some constants
MAX_ITER = 10;
num_iterations = 1;

%Initializing sequences 
mu_seq = zeros(1, MAX_ITER);
om_seq = zeros(1, MAX_ITER);
eta_seq = zeros(1, MAX_ITER);
x_seq = zeros(MAX_ITER, n);
lm_seq = zeros(MAX_ITER, n);
obj_seq = zeros(1, MAX_ITER);

mu_seq(1) = 10;
om_seq(1) = 1/mu_seq(1);
eta_seq(1) = 1 / mu_seq(1)^0.1;



for k = 1:MAX_ITER
    x_seq(k, :) = SolveSubproblem();
    
    % Calculating constraint for solution to subproblem
    [constr_vec, constr_norm] = CalculateConstraint(x_seq(k), a_vecs, d_vecs);
    
    if constr_norm <= eta_seq(k)
        % Test for convergence
        proj_diff = CalculateProjectionDiffNorm();
        if (constr_norm <= eta_star) && (proj_diff <= om_star)
            break;
        end

        % Update multipliers, tighten tolerances
        lm_seq(k+1, :) = lm_seq(k, :) - mu_seq(k) * constr_vec;
        mu_seq(k+1) = mu_seq(k);
        eta_seq(k+1) = eta_seq(k) / (mu_seq(k+1)^0.9);
        om_seq(k+1) = om_seq(k) / mu_seq(k+1);

    else
        % Increase penalty parameter, tighten tolerances
        lm_seq(k+1, :) = lm_seq(k, :);
        mu_seq(k+1) = 100 * mu_seq(k);
        eta_seq(k+1) = 1/ (mu_seq(k+1)^0.1);
        om_seq(k+1) = 1/ (mu_seq(k+1));

    end
    % Should be incremented RIGHT after the next calcualtion step
    num_iterations = num_iterations + 1;
    obj_seq(k) = dot(c, x_seq(k));
end

% Resize x_sequence and objective according to number of iterations
x_seq(:, :) = x_seq(num_iterations, :);
obj_seq(:) = obj_seq(num_iterations);


end