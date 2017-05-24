 function Irot = rotate_image(I,angle)
 % 
 %   This function takes an image I and creates a new version of the image
 %   which is rotated by amount angle
 %
 % arguments:
 %
 %   I - the original grayscale image, stored as a matrix
 %   angle - the amount by which to rotate the image counter-clockwise, in degrees
 %
 % return value:
 %   
 %   Irot - an image which containing the rotated original
 
        miny = -size(I,1)/2;
        maxy = size(I,1) - size(I,1)/2 - 1;
        
        minx = -size(I,2)/2;
        maxx = size(I,2) - size(I,2)/2 - 1;
        
        [xcoord,ycoord] = meshgrid(minx:maxx,miny:maxy);
        X = [xcoord(:) ycoord(:)]';
        Y = rotate(X, angle);
        
        new_minx = ceil(min(Y(1,:)));
        new_maxx = ceil(max(Y(1,:)));
        
        new_miny = ceil(min(Y(2,:)));
        new_maxy = ceil(max(Y(2,:)));
        [newxcoord, newycoord] = meshgrid(new_minx:new_maxx, new_miny:new_maxy);
        Ivector = I(:)';
        Irotated = griddata(Y(1,:), Y(2,:), Ivector, newxcoord(:), newycoord(:));
        Irot = reshape(Irotated, size(newxcoord));
 end