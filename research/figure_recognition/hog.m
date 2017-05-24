function ohist = hog(I)
  %
  % compute orientation histograms over 8x8 blocks of pixels
  % orientations are binned into 9 possible bins
  %
  % I : grayscale image of dimension HxW
  % ohist : orinetation histograms for each block. ohist is of dimension (H/8)x(W/8)x9
  %
  [h,w] = size(I);
  h2 = ceil(h/8);           % hog size.
  w2 = ceil(w/8);
  nori = 9;                 % number of orientation bins

  % use a gradient operator to approximate local gradient and its direction 
  % from which edge location and orientation can be retrieved.
  [mag,ori] = mygradient(I);
  thresh = 0.1*max(mag(:));   % tenth of the maximum gradient.
  edges = im2bw(mag,thresh);

  % separate out pixels into orientation channels
  ohist = zeros(h2,w2,nori);
  for i = 1:nori
    % create a binary image containing 1's for the pixels that are edges at this orientation
    w = pi/nori; at_ori = intersect(find(ori >= (i - 1)*w - pi/2),find(ori < i*w - pi/2));
    B = zeros(size(edges)); B(at_ori) = edges(at_ori);

    % compute orientation distribution for each 8x8 blocks.
    chblock = im2col(B,[8 8],"distinct");
    ohist(:,:,i) = reshape(sum(chblock), h2, w2); %sum over each block and store result in ohist
  end

  % normalize the histogram so that sum over orientation bins is 1 for each block
  cdf = sum(ohist, 3);
  
  % compute normalizing factor and fixup bad values.
  norm = 1./cdf;
  norm(cdf == 0) = 0;
  
  % normalize.
  for i = 1:nori
    ohist(:,:,i) = ohist(:,:,i).*norm;
  end
endfunction