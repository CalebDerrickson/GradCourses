function quad_approx = QuadModel(f_k, g_k, B_k, p_k)

const = f_k;
linear = dot(g_k, p_k);
quad = 0.5*dot(p_k', mtimes(B_k, p_k));

quad_approx = const + linear + quad;

end