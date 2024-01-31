function hw4p1p3(x0, niter, eps)
global func_counter
global lin_systems_counter
func_counter = 0;
lin_systems_counter = 0;

if isempty( func_counter )
    func_counter=0; %Initializing counter
end

if isempty( lin_systems_counter )
    lin_systems_counter=0; %Initializing counter
end

[~, TR_seq] = TrustRegion(@rosenbrockfgH, niter, x0, eps);

fprintf("Number of function evaluations: %d\n", func_counter);
fprintf("Number of linear systems solved: %d\n", lin_systems_counter);
end