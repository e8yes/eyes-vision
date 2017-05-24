function [x,y,score] = detect(I,template,ndet)
  %
  % return top ndet detections found by applying template to the given image.
  %   x,y should contain the coordinates of the detections in the image
  %   score should contain the scores of the detections
  %


  % compute the feature map for the image
  f = hog(I);

  nori = size(f,3);

  % cross-correlate template with feature map to get a total response
  R = zeros(size(f,1),size(f,2));
  for i = 1:nori
    R = R + imfilter(f(:,:,i), template(:,:,i), "replicate");
  end
  
  % now return locations of the top ndet detections
  % sort response from high to low
  [val,ind] = sort(R(:),"descend");

  % work down the list of responses, removing overlapping detections as we go
  i = 1;
  detcount = 1;
  while ((detcount < ndet) & (i < length(ind)))
    % convert ind(i) back to (i,j) values to get coordinates of the block
    [yblock, xblock] = ind2sub(size(f), ind(i));

    assert(val(i)==R(yblock,xblock)); %make sure we did the indexing correctly

    % now convert yblock,xblock to pixel coordinates 
    ypixel = yblock*ceil(size(I,1)/size(f,1));
    xpixel = xblock*ceil(size(I,2)/size(f,2));

    % check if this detection overlaps any detections which we've already added to the list
    overlap = false;
    sizeby = size(template, 1)*ceil(size(I,1)/size(template,1));
    sizebx = size(template, 2)*ceil(size(I,1)/size(template,2));
    minx = xpixel; maxx = xpixel + sizebx;
    miny = ypixel; maxy = ypixel + sizeby;
    j = 1;
    while j < detcount
      x0 = max(minx, x(j)); x1 = min(maxx, x(j) + sizebx);
      y0 = max(miny, y(j)); y1 = min(maxy, y(j) + sizeby);
      
      if (x0 < x1 && y0 < y1)
        overlap = true;
        break;
      endif
      j = j + 1;
    end

    % if not, then add this detection location and score to the list we return
    if (~overlap)
      x(detcount) = xpixel
      y(detcount) = ypixel
      score(detcount) = val(i);
      detcount = detcount + 1;
    end
    i = i + 1;
  end
endfunction

