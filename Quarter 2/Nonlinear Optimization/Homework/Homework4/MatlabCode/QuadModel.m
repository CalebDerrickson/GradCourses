function quad_approx = QuadModel(f_k, g_k, B_k, p_k)

quad_approx = f_k + dot(g_k, p_k) + 0.5*dot(p_k, mtimes(B_k, p_k));

end