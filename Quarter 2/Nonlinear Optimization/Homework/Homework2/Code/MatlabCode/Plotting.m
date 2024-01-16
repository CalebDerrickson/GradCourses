function  Plotting(x0, c, g_seq)
    figure(1)

    plot(g_seq)
    xlabel('step'), ylabel('$ || \nabla f(x_k) || $','Interpreter','latex') ;
    title(strcat('$ || \nabla f(x_k) || $ for Initial Condition  [',  num2str(x0), ']' ), 'Interpreter','latex')
    grid on;
    

    ratio = zeros(length(g_seq)-1, 1);

    figure(2)
    x0 = [1, 1];
    ratio = zeros(length(g_seq)-1, 1);
    for i = 1:length(g_seq)-1
        ratio(i) = g_seq(i+1)/g_seq(i);
    end
    plot(ratio)
    xlabel('step'), ylabel('Value') ;
    ylim([0 0.75])
    title(strcat('Ratio of Gradient Sequence for Initial Condition  [',  num2str(x0), ']' ), 'Interpreter','latex')
    grid on;
end