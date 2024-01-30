function [TR_seq, Delta_seq, Rho_seq] = TrustRegion(func_handle, niter, x0, eps)

func = @(x, varargin) func_handle(x, varargin{:});

% PARAMETERS
delta_hat = 1;                    % Arbitary, need > 0
TOLERANCE = 0.25;        % The reduction ratio cutoff to be considered "good". % 0 < TOLERANCE <= 1
eta = TOLERANCE*0.8;    % 0 <= eta < TOLERANCE
max_size = 1;                      % for resizing the array afterwards

TR_seq = zeros(niter, max(size(x0)));
Rho_seq = zeros(niter, 1);
Delta_seq = zeros(ntier, 1);

TR_seq(1, :) = x0;
Delta_seq(1) = delta_hat * 0.5;


for i = 1:niter
    
    % Getting search direciton from Dogleg
    p_k = Dogleg(func_handle, TR_seq(i, :), Delta_seq(i, :));

    % Performance ratio
    Rho_seq(i) = reduction_ratio(@rosenbrockfgH, @QuadModel, TR_seq(i, :), p_k);

    % Trust region update
    if Rho_seq(i) < TOLERANCE
        Delta_seq(i+1) = Delta_seq(1) * TOLERANCE;
    
    else %Ratio is "good"
        if (Rho_seq(i) > 1 - TOLERANCE) && (norm(p_k) == Delta_seq(i))
            Delta_seq(i+1) = min(2 * Delta_seq(i), delta_hat);
        else 
            Delta_seq(i+1) = Delta_seq(i);
        end
    end
    
    % Iterate update
    if Rho_seq(i) > eta
        TR_seq(i+1, :) = TR_seq(i, :) + p_k;
    else 
        TR_seq(i+1, :) = TR_seq(i, :);
    end

    max_size = i;

% Quick gradient check
[~, g, ~] = func(TR_seq(i+1, :), max(TR_seq(i+1, :)), '', 'g');
if norm(g) < eps
    break
end

end  % end(for)

if (max_size ~= 1) || (max_size ~= niter)
    % Only want to return nonzero entries
    TR_seq = TR_seq(1:max_size, :);
    Delta_seq = Delta_seq(1:max_size);
    Rho_seq = Rho_seq(1:max_size);
end

end