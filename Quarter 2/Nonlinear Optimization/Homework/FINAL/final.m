function [x, obj] =  final(m, n, s, p)


%TODO
% x Write CalculateConstraint function
% x Write CalculateProjectionDiffNorm function
% x Write SolveSubproblem function (hard)
%       x Write Algorithm 16.1 or 16.2
% x Understand what to pass into AUGLAG
% x Setup random instances
% x Form the problem to pass into AUGLAG
% ? Done?

% ADDING UTILS TO PATH. MIGHT NOT WORK ON NON-WINDOWS MACHINES
addpath("./Utils/");
addpath("./Utils/Subproblem/");

rng(s);


[c, q_0, d_vecs, a_vecs] = GetRandomVariables(m, n);

% Temp value
gamma = 0;

l = -1.*ones(1, n);
u = ones(1, n);

[x_seq, obj_seq] = AUGLAG(c, diag(q_0), a_vecs, d_vecs, gamma, l, u, p);
x = x_seq(end, :);
obj = obj_seq(end);

end
