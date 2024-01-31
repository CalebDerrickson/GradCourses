function  [x] = hw4p1p5(x0, niter, eps)

[~,name,~]=fileparts(pwd);
if strcmp(name, 'MatlabCode') ~= 0
    cd ("..\")
end

cd('MatlabCode\Dogleg')

[~, TR_seq] = TrustRegion(@rosenbrockfgH, x0, niter, eps);
x = TR_seq(end, :);
end