function plotting(seq, stepsize)

    numsteps = size(seq, 1);
    k = length(seq(:, 1));
    xaxis = 1:k;
    colorMap = turbo(numsteps);
    
    subplot(1, 3, 1);
    plot(xaxis, seq(:, 1) );
    xlabel('step'), ylabel('x - position'), title(strcat('X - Position of sequence, stepsize =  ', num2str(stepsize)))
    grid on

    subplot(1, 3, 2);
    plot(xaxis, seq(:, 2) );
    xlabel('step'), ylabel('y - position') ,title(strcat('Y - Position of sequence, stepsize =  ',  num2str(stepsize)))
    grid on

    subplot(1, 3, 3);
    % Plot each step with a changing color
    scatter(seq(:, 1), seq(:, 2), 50, colorMap, 'filled');
    
    clim([0, numsteps]);
    c = colorbar;
    c.Label.String = 'Step';
    xlabel('x - position'), ylabel('y - position'), title(strcat('Convergence of Sequence, stepsize =  ',  num2str(stepsize)))
    grid on


end