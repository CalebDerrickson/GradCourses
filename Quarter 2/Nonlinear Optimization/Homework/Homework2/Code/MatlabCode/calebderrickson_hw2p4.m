function calebderrickson_hw2p4(x0, c)
    % This is the "main function" of this program. 
    %
    % An acceptable call to this function would be 
    % calebderrickson_hw2p4([1, 1], 3)
    %
    % This will output TWO plots:
    %   - The gradient iteration
    %   - The ratio between iterates in sequence
    %
    % The program will also print out an estimate
    % of the minima, after some number of iterations.
    %
    % The tolerance is set fairly high: 1e-4.
    
    clc
    [seq, g_seq] = SteepestDescentWithBacktracking(x0, c);
    Plotting(x0, c, g_seq)
    
end