function [x_seq, obj_seq] =  AUGLAG(c, Q_0, a_vecs, d_vecs, gamma, l, u, p)

[m, n] = size(Q_0);
% Choose an initial point x0 and initial multipliers lm0
% The origin is given as a feasible point. 

% Choose convergence tolerances eta_star and om_star
% Don't know what these should exactly by atm. Temp values.
eta_star = 10e-3;
om_star = 10e-4;
10e-3
% Some constants
num_iterations = 1;

%Initializing sequences 
mu_seq = zeros(1, p);
om_seq = zeros(1, p);
eta_seq = zeros(1, p);
x_seq = zeros(p, n);
lm_seq = zeros(p, n);
obj_seq = zeros(1, p);

mu_seq(1) = 10;
om_seq(1) = 1/mu_seq(1);
eta_seq(1) = 1 / mu_seq(1)^0.1;



for k = 1:p
    x_seq(k+1, :) = SolveSubproblem(x_seq(k, :), c, Q_0, d_vecs, a_vecs, mu_seq(k), lm_seq(k, :), gamma, l, u);
    
    % Calculating constraint for solution to subproblem
    [constr_vec, constr_norm] = CalculateConstraint(x_seq(k+1), a_vecs, d_vecs);
    
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