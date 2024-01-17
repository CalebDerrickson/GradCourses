function [seq, g_seq] = SteepestDescentWithBacktracking(x0, c)
    %Initializing variables / output
    MAX_SIZE = 1e4;
    seq = zeros(MAX_SIZE, 2);
    seq(1, :) = x0;
    epsilon = 1e-4;
    [f_eval, g_eval] = FunctionEvaluation(x0, c); 
    g_seq = zeros(MAX_SIZE, 1);

   g_seq(1, :) = norm(g_eval);

    % Main Loop
    i = 1;
    while (norm(g_eval) > epsilon && i < MAX_SIZE)
   
       alpha = Backtracking( seq(i, :) , c);

       next_iter = seq(i, :) - alpha * g_eval;
       seq(i+1, :) = next_iter;
       
       [f_eval, g_eval] = FunctionEvaluation(seq(i+1, :), c);
       g_seq(i+1, :) = norm(g_eval);

        i = i + 1;
    end

    % Adjusting arrays so their sizes make sense. 
    % Minimizing the number of memory allocation calls.

    if i < MAX_SIZE
        new_seq = zeros(i, 2);
        new_seq = seq(1:i, :);
        clear seq;
        seq = new_seq;

        new_gseq = zeros(i, 1);
        new_gseq = g_seq(1:i, :);
        clear g_seq;
        g_seq = new_gseq;
    end

    fprintf("Estimate value for minima: [%d, %d]\n", seq(i, 1), seq(i, 2))
    fprintf("Number of iterations: %d", i);
end