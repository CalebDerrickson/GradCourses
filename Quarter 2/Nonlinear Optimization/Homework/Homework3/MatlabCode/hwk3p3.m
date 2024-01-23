function  hwk3p3 = hwk3p3(x0, niter, eps)

    clc
    % Fenton Function Sequences
    VN_seq = VN(@fentonfgH, x0, niter, 1);
    NMHM_seq = NMHM(@fentonfgH, x0, niter, 1, eps);
    
    figure(1);  
    plot(NMHM_seq); % Y coord converges to the negative of the VN_seq. Bug, or another minima?
    figure(2);  
    plot(VN_seq);

end