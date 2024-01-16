function [func, grad] = FunctionEvaluation(x, c)
    
    %Writing out given function and evaluating it 
    f = @(x) x(1)^2 + c*x(2)^2;
    
    func = f(x);

    %Same for gradient
    g1 = @(x) 2*x(1);
    g2 = @(x) 2*c*x(2);

    grad = [g1(x), g2(x)];

end