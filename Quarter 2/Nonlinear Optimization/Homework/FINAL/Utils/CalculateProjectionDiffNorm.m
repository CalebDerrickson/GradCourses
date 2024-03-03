function diff_norm = CalculateProjectionDiffNorm(x_k, grad_lag_k, l, u)

x_proj = Projection(x_k - grad_lag_k, l, u);
diff_norm = norm(x_proj - x_k);

end