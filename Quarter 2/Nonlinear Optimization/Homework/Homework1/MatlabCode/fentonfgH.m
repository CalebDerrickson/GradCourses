function [f, g, H] = fentonfgH(x)

    %Implementation of evaluating the fenton function at a given point
    %as well as its gradient and Hessian.
    
    %The parameter x should be an array of size 2.
    
    %Evaluating f at x
    func = @(x) (1/10) * (12 + x(1)^2 + (1 + x(2)^2)/x(1)^2 + (x(1)^2 * x(2)^2 + 100) / (x(1)^4 * x(2)^4) );
    f = func(x);
    
    %Hard coding the gradient, and evaluating at x
    g1 = @(x) 0.2*x(1) - 0.2*(x(2)^2 + 1)/x(1)^3 + 0.2/(x(1)^3*x(2)^2) - 0.4*(x(1)^2*x(2)^2 + 100)/(x(1)^5*x(2)^4);
    g2 = @(x) 0.2*x(2)/x(1)^2 + 0.2/(x(1)^2*x(2)^3) - 0.4*(x(1)^2*x(2)^2 + 100)/(x(1)^4*x(2)^5);

    g = [g1(x), g2(x)];
    
    %Hard Coding the Hessian, then evaluating
    H11 = @(x)  0.2 + 0.6*(x(2)^2 + 1)/x(1)^4 - 1.4/(x(1)^4*x(2)^2) + 2.0*(x(1)^2*x(2)^2 + 100)/(x(1)^6*x(2)^4);
    H12 = @(x) -0.4*x(2)/x(1)^3 - 1.2/(x(1)^3*x(2)^3) + 1.6*(x(1)^2*x(2)^2 + 100)/(x(1)^5*x(2)^5);
    H21 = @(x) -0.4*x(2)/x(1)^3 - 1.2/(x(1)^3*x(2)^3) + 1.6*(x(1)^2*x(2)^2 + 100)/(x(1)^5*x(2)^5);
    H22 = @(x) 0.2/x(1)^2 - 1.4/(x(1)^2*x(2)^4) + 2.0*(x(1)^2*x(2)^2 + 100)/(x(1)^4*x(2)^6);

    H = [
        H11(x), H12(x);
        H21(x), H22(x) 
    ];
end