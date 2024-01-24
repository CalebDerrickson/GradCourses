function hwk3p3 = hwk3p3(x0, niter, eps)

    

    clc 
    sz = size(x0);
    NMHM_seq = NMHM(@rosenbrockfgH, x0, niter, sz(2), eps);
    hwk3p3 = NMHM_seq(end, :);
end