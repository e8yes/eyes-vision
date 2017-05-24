function [path] = shortest_path(costs)
  %
  % given a 2D array of costs, compute the minimum cost vertical path
  % from top to bottom which, at each step, either goes straight or
  % one pixel to the left or right.
  %
  % costs:  a HxW array of costs
  %
  % path: a Hx1 vector containing the indices (values in 1...W) for 
  %       each step along the path
  %
  %
  %
  
  % Boundary condition.
  for j=1:size(costs, 2)
    c(1, j) = costs(1, j);
  end
  
  % Propagate.
  for i=2:size(costs, 1)
    for j=1:size(costs, 2)
      if j == 1
        m(1) = Inf;
      else
        m(1) = c(i - 1, j - 1);
      end
      
      if j == size(costs, 2)
        m(3) = Inf;
      else
        m(3) = c(i - 1, j + 1);
      end
      
      m(2) = c(i - 1, j);
      
      c(i, j) = min(m) + costs(i, j);
    end
  end
  
  % Reconstruct the path.
  path = zeros(size(costs, 1));
  lmin = find(c(size(costs, 1), :) == min(c(size(costs, 1), :)))(1);
  path(size(costs, 1)) = lmin;
  
  for k=(size(costs, 1) - 1):-1:1
    if lmin == 1
        m(1) = Inf;
      else
        m(1) = c(k, lmin - 1);
      end
      
      if lmin == size(costs, 2)
        m(3) = Inf;
      else
        m(3) = c(k, lmin + 1);
      end
      
      m(2) = c(k, lmin);
      
      lmin = lmin + (find(m == min(m))(1) - 2);
      path(k) = lmin;
  end
endfunction
