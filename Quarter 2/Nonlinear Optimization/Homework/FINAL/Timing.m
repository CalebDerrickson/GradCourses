function Timing()

iterations = 10; % per dimension
scale = 20;
sizemax = 200/scale;

times = zeros(sizemax, iterations);

gamma = 200;
p = 50;

addpath("./Utils/");
addpath("./Utils/Subproblem/");

for i = 1:sizemax
    m = scale*i;
    n = 2*m;

    l = -1.*ones(1, n);
    u = ones(1, n);

    for j = 1:iterations
        [c, q_0, d_vecs, a_vecs] = GetRandomVariables(m, n);
    
        tic
        [~, ~] =  AUGLAG(c, diag(q_0), a_vecs, d_vecs, gamma, l, u, p);
        times(i, j) = toc;
        
    end
    fprintf("%d\n", round(i/sizemax, 2)*100);
end


avg_time = zeros(1, sizemax);
stdev_time = zeros(1, sizemax);

for i = 1:sizemax
    avg_time(i) = mean(times(i, :));
    stdev_time(i) = std(times(i, :));
end


range = linspace(scale, sizemax*scale, sizemax);
figure(1)
hold on;
plot(range, avg_time, LineWidth=2);

title("Average time, gamma = " + num2str(gamma));
xlabel("Dimension");
ylabel("Time");
grid on;


figure(2)
hold on;
plot(range, stdev_time, LineWidth=2);

title("Standard Deviation, gamma = " + num2str(gamma));
xlabel("Dimension");
ylabel("Time");
grid on;



end