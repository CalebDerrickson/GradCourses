function alpha = FindAlpha(x, dx, s, ds, gamma, sigma)

alphas = zeros(1, length(x));
quad = ds.* dx;

mu = dot(x, s) / length(x);

lin = s .* dx + x .*ds + (gamma * (1 - sigma)*mu);
const = x .* s - (gamma * mu);

f = @(alpha, i) alpha^2 *quad(i) + alpha*lin(i) + const(i);
df = @(alpha, i) 2*alpha* quad(i) + lin(i);
D = @(i) lin(i)^2 - 4*quad(i) * const(i);


for i = 1:length(x)
    if quad(i) > 0
        if D(i) <= 0
            alphas(i) = 1;
        else 
            if df(0, i) > 0
                alphas(i) = 1;
            else
                %Find roots to f
                xs = roots([quad(i), lin(i), const(i)]);
                alphas(i) = min([1, find(xs >=0)']);
            end
        end
    elseif quad(i) < 0
        if D(i) <= 0
            % error
            alphas(i) = nan;
        else 
            xs = roots([quad(i), lin(i), const(i)]) ;
            alphas(i) = min([1, find(xs >= 0)']);
        end
    else
        %error
        alphas(i) = nan;
    end
end

alpha = min(alphas);
end