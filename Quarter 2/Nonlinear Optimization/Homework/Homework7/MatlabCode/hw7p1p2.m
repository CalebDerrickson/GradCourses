function hw7p1p2()

iterations = 10; % per dimension
scale = 50;
sizemax = 1000/scale;

times_simplex = zeros(sizemax, iterations);
times_interior = zeros(sizemax, iterations);


for i = 1:sizemax
    m = scale*i;
    n = 2*m;
   
    for j = 1:iterations
        A = randMatrix(m, n);
        e = [ones(1, m) zeros(1, n - m)];
        b = randSolve(A, e);
        c = randObjective(A);


        tic
        [~, ~] = simplex(A, b, c, e, 100);
        times_simplex(i, j) = toc;

        e = ones(1, n);
        b = randSolve(A, e);
        [c, u] = randObjective(A);
        
        x0 = e;
        lm0 = mtimes(inv(mtimes(A, A')), b);
        s0 = 100.*u;
        
        u_avg = mean(u);
        gamma = min((1/u_avg).*u);
        
        tic
        [~ ,~] = InteriorPoint(A, b, c, gamma, 0.1, 0.9, x0, lm0, s0);
        times_interior(i, j) = toc;
        
    end
    fprintf("%d\n", round(i/sizemax, 2)*100);
end


avg_time_simplex = zeros(1, sizemax);
avg_time_interior = zeros(1, sizemax);

for i = 1:sizemax
    avg_time_simplex(i) = mean(times_simplex(i, :));
    avg_time_interior(i) = mean(times_interior(i, :));
end


range = linspace(scale, sizemax*scale, sizemax);
figure(1)
hold on;
plot(range, avg_time_simplex, LineWidth=2, Tag="Simplex");
plot(range, avg_time_interior, LineWidth=2, Tag = "Interior Point");
title("Average time");
xlabel("Dimension");
ylabel("Time");
legend('Simplex', 'Interior Point');
grid on;
end