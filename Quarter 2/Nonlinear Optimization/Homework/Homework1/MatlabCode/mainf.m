function mainf(x, k, stepsize)
    
    %This will act as a pseudo main file to run everything
    %The inputs are the initial conditions and the number of steps in
    %the newton's method

    % x - initial conditions
    %   Should be a 1x2 array
    % k - number of steps
    %   Should be an integer > 1

    clc
    seq = newtonseq(x, k, stepsize);
    
    plotting(seq, stepsize);

end