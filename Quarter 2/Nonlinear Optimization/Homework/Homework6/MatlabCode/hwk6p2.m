function hwk6p2()
clc
global degen_counter

degen_counter = 0;

if isempty( degen_counter )
    degen_counter=0; %Initializing counter
end



sizemax = 2000/50;
iterations = 10;
times = zeros(20, 10);
degen_arr = zeros(1, sizemax);

for i = 1:sizemax
    m = 50*i;
    n = 2*m;
   
    for j = 1:iterations
        A = randMatrix(m, n);
        e = [ones(1, m) zeros(1, n - m)];
        b = randSolve(A, e);
        c = randObjective(A);
        tic
        [~, ~] = simplex(A, b, c, e, 100);
        times(i, j) = toc;
    end
    degen_arr(i) = degen_counter;
    fprintf("%d\n", round(i/sizemax, 2)*100);
end


avg_time = zeros(1, sizemax);
std_time = zeros(1, sizemax);

for i = 1:sizemax
    avg_time(i) = mean(times(i, :));
    std_time(i) = std(times(i, :));
end


range = linspace(50, sizemax*50, sizemax);
figure(1)
plot(range, avg_time, LineWidth=2);
title("Average time");
xlabel("Dimension");
ylabel("Time");
grid on;

figure(2)
plot(range, std_time, LineWidth=2);
title("Variance time");
ylabel("Time Variance")
xlabel("Dimension")
grid on;

figure(3)
plot(range, degen_arr, LineWidth=2);
title("Degenerate solutions");
ylabel("Number")
xlabel("Dimension")
grid on;

end