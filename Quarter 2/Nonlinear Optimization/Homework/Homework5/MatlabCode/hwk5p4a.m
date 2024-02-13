function hwk5p4a(eps, niter)
clc

global func_counter
func_counter = 0;

if isempty( func_counter )
    func_counter=0; %Initializing counter
end


start_dim = 30;
end_dim = 500;

func_counters = zeros(end_dim- start_dim -1, 1);
x0 = 2.*ones(end_dim, 1);

for i = start_dim:end_dim
    [~] = BFGS(x0(1:i), eps, @rosenbrockfgH, niter);
    func_counters(i- start_dim +1) = func_counter;
    func_counter = 0;
    fprintf("%f\n", round((i - start_dim)/(end_dim - start_dim)*100, 3));
end

range = start_dim:end_dim;
plot(range,func_counters);
grid on;
title("Increasing Dimensions and Function Evaluations");
xlabel("Dimension size");
ylabel("Function Evaluations");

end