function H_update = HessianUpdate(H, s_k, y_k)

    %Ensure s_k and y_k are column vectors
    if isrow(s_k) s_k = s_k'; end
    if isrow(y_k) y_k = y_k'; end


    %Define theta_k
    theta_k = 1;
    small = dot(s_k, y_k);
    big = dot(s_k, mtimes(H, s_k));
    if small < 0.2*big
        theta_k = (0.8*big) / (big - small);
    end

    %Define r_k
    r_k = theta_k.*y_k + (1 - theta_k)*mtimes(H, s_k);

    %Now update H
    top = dot(mtimes(H, s_k), mtimes(s_k', H));
    H_update = H - top / big;
    
    top = r_k * r_k';
    H_update = H_update + (1/dot(s_k, r_k)) * top;

end