function ratio = reduction_ratio(func_handle,approx_handle, x_k, p_k)

func = @(x, varargin) func_handle(x, varargin{:});
func_a = @(x, varargin) approx_handle(x, varargin{:});

[func_x_k, ~, ~] = func(x_k, max(size(x_k)), '', 'f');
[func_p_k, ~, ~] = func(x_k + p_k, max(size(x_k)), '', 'f');

[pre_zero, ~, ~] = func(0, max(size(x_k)), '', 'f');
[pre_p_k, ~, ~] = func(p_k, max(size(x_k)), '', 'f');

actual =  func_x_k - func_p_k;
predicted = pre_zero - pre_p_k;

ratio = actual / predicted;
end