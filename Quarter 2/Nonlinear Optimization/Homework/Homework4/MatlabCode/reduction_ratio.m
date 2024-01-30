function ratio = reduction_ratio(func_handle,approx_handle, x_k, p_k)

func = @(x, varargin) func_handle(x, varargin{:});
func_a = @(x, varargin) approx_handle(x, varargin{:});

[func_x_k, g_k, H_k] = func(x_k, max(size(x_k)), '', 'everything');
[func_p_k, ~, ~] = func(x_k + p_k, max(size(x_k)), '', 'f');

pre_zero = func_a(0, g_k, H_k, p_k);
pre_p_k = func_a(p_k, g_k, H_k, p_k);

actual =  func_x_k - func_p_k;
predicted = pre_zero - pre_p_k;

ratio = actual / predicted;
end