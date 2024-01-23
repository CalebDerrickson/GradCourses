function VN_seq = VN(funcfgH, x0, niter, n)

%Parameters
%   funcfgH - funciton we are analyzing.
%                   Assumed to have continuous gradient and Hessian.
%   x0 - initial step. Should be 1 x 2 i.e. an array of size 2.
%   niter - number of iterations
%   n - Value for Rosenbrock order function

% Initialize array with zeros
VN_seq = zeros(niter, 2);

VN_seq(1, :) = x0;

%Populate the array element-wise
for i = 1:niter-1
    [~, g, H] = funcfgH(VN_seq(i, :), n);

    % Compute QR of H for inverse
    [Q, R] = qr(H);

    % Solving QRx = b => Rx = Q^T b
    b = Q' * g';
    x = fixed.backwardSubstitute(R, b);

    % Update sequence
    VN_seq(i+1, :) = VN_seq(i, :) - x';
end