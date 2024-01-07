function newtonseq = newtonseq(x0, k, stepsize)
    %Funcition to calculate the newton's method sequence up to step k
    %of the initial conditions x0 given.

    %k should be an integer > 1
    %x0 should be a 1x2 array of real numbers
    
    % Initialize the array with zeros
    newtonseq = zeros(k, 2);

    newtonseq(1, :) = x0;

    % Populate the array element-wise
    for i = 1:k-1
        [~, g, H] = fentonfgH(newtonseq(i, :));
        newtonseq(i+1, :) = newtonseq(i, :) - stepsize * (H\g')';
    end
end