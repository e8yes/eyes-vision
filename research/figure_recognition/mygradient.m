function [mag,ori] = mygradient(I)
  %
  % compute image gradient magnitude and orientation at each pixel
  %
  % Sobel kernel.
  kernel_x = [1 0 -1; 2 0 -2; 1 0 -1];
  kernel_y = [1 2 1; 0 0 0; -1 -2 -1];
  dx = imfilter(I, kernel_x, "replicate");
  dy = imfilter(I, kernel_y, "replicate");
  
  mag = sqrt(dx.*dx + dy.*dy);
  ori = atan(dy./dx);
  
  % fix up bad values at vertical gradients, or horizontal edges.
  ori(intersect(find(dx == 0),find(dy >= 0))) = pi/2;
  ori(intersect(find(dx == 0),find(dy < 0))) = -pi/2;
end