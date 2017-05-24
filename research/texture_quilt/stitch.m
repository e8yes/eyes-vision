function [result] = stitch(leftI,rightI,overlap);
  % 
  % stitch together two grayscale images with a specified overlap
  %
  % leftI : the left image of size (H x W1)  
  % rightI : the right image of size (H x W2)
  % overlap : the width of the overlapping region.
  %
  % result : an image of size H x (W1+W2-overlap)
  %
  if (size(leftI,1)~=size(rightI,1)); % make sure the images have compatible heights
    error('left and right image heights are not compatible');
  end
  
  h = size(leftI, 1);
  
  % compute cost map.
  left_over = leftI(:, (size(leftI,2) - overlap + 1):end);
  right_over = rightI(:, 1:overlap);
  cost_map = abs(left_over - right_over);
  
  % split.
  path = shortest_path(cost_map);
  
  all_over = [left_over(1, 1:path(1)) right_over(1, (path(1) + 1):end)];
  for i = 2:h
    row = [left_over(i, 1:path(i)) right_over(i, (path(i) + 1):end)];
    all_over = [all_over; row];
  end
  result = [leftI(:, 1:(size(leftI,2) - overlap)) all_over rightI(:, (overlap + 1):end)];
endfunction