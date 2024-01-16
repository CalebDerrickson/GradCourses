function calebderrickson_hw2p4(x0, c)
    
    clc
    [seq, g_seq] = SteepestDescentWithBacktracking(x0, c);
    Plotting(x0, c, g_seq)
    
end