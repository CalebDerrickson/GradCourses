function [x, obj] =  final(m, n, s, p)


%TODO
% x Write CalculateConstraint function
% x Write CalculateProjectionDiffNorm function
% - Write SolveSubproblem function (hard)
%       - Write Algorithm 16.1 or 16.2
% - Understand what to pass into AUGLAG
% x Setup random instances
% - Form the problem to pass into AUGLAG
% - Done?

% ADDING UTILS TO PATH. MIGHT NOT WORK ON NON-WINDOWS MACHINES
addpath("./Utils/");
addpath("./Utils/Subproblem/");

rng(s);


[c, q_0, d_vecs, a_vecs] = GetRandomVariables(m, n);

% Temp value
gamma = 1;

[x_seq, obj_seq] = AUGLAG(c, diag(q_0), a_vecs, d_vecs, gamma);
x = x_seq(end, :);
obj = obj_seq(end);

end
