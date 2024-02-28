function [c, u] = randObjective(A)

% Get size of objective function
% Set random distribution vector.
% Initialize the obj function
% Add obj function with random distribution. 

[~, n] = size(A);
u = rand(n, 1);

c = ones(n, 1);

c = (c + 100.*u)';


end