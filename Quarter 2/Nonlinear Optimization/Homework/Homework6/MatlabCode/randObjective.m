function c = randObjective(A)

% Get size of objective function
% Set random distribution vector.
% Initialize the obj function
% Add obj function with random distribution. 

n = size(A(:, 1));
u = randn(n, 1);

c = ones(n, 1);

c = (c + 50.*u)';


end