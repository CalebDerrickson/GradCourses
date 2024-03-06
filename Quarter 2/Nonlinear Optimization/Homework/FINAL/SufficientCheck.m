function SufficientCheck()

% ADDING UTILS TO PATH. MIGHT NOT WORK ON NON-WINDOWS MACHINES
addpath("./Utils/");
addpath("./Utils/Subproblem/");

iterations = 10; % per dimension
scale = 20;
sizemax = 200/scale;
p = 50;


% Temp value
gamma = 200;

opt_seq = zeros(sizemax, iterations);

for i = 1:sizemax
    m = scale*i;
    n = 2*m;

    l = -1.*ones(1, n);
    u = ones(1, n);

    for j = 1:iterations
        [c, q_0, d_vecs, a_vecs] = GetRandomVariables(m, n);
    
        tic
        [x_seq, ~, lm_seq, mu_seq] =  AUGLAG(c, diag(q_0), a_vecs, d_vecs, gamma, l, u, p);
        x = x_seq(end, :);
        lm = lm_seq(end, :);
        mu = mu_seq(end);

        if OPTCheck(x, diag(q_0), a_vecs, d_vecs, gamma, lm, mu)
            opt_seq(i, j)  = 1;
        end
    end
    fprintf("%d\n", round(i/sizemax, 2)*100);
end

avg_opt = zeros(1, sizemax);

for i = 1:sizemax
    avg_opt(i) = mean(opt_seq(i, :));
end

range = linspace(scale, sizemax*scale, sizemax);
figure(1)
hold on;
plot(range, avg_opt, LineWidth=2);

title("Average Sufficient check, gamma = " + num2str(gamma));
xlabel("Dimension");
ylabel("Number of Sufficient checks");
grid on;


end