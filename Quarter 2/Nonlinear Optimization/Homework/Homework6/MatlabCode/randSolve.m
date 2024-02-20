function b = randSolve(A, e)

if isrow(e) e = e'; end

b = mtimes(A, e);

end