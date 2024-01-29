function hw4p1p2(niter)


%Dense
a_n = zeros(1, niter);

%Sparse
b_n = zeros(1, niter);
b = ones(1, 199+niter);
eval = 2.*b;

for i = 1:niter
    
 
    [~, ~, H_dense] = rosenbrockfgH(eval(1:i+199), i+199, 'dense');
    [~, ~, H_sparse] = rosenbrockfgH(eval(1:i+199), i+199, 'sparse');
    tic;
    x = H_dense \ b(1:i+199)';
    a_n(i) = toc;
    tic;
    x = H_sparse \ b(1:i+199)';
    b_n(i) = toc;
    %disp(i/niter*100)
end

lspace = linspace(200, niter + 200, niter);
figure(1)
plot(lspace, a_n, DisplayName='Dense', LineWidth=2);
hold on;
plot(lspace, b_n, DisplayName='Sparse', LineWidth=2);
grid on;
legend

title("Matrix Multiplicaiton Timings")
ylabel("Time (s)")
xlabel("Size")
hold off;

end