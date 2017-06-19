function X = reconstruct()
        threshold = 0.02;
        [Lv_C, Lv_goodpixels] = decode('left/left_',1,20,threshold);
        figure(1);
        imshow(Lv_C/1023 .* Lv_goodpixels);
        
        [Lh_C, Lh_goodpixels] = decode('left/left_',21,40,threshold);
        figure(2);
        imshow(Lh_C/1023 .* Lh_goodpixels);
        
        [Rv_C, Rv_goodpixels] = decode('right/right_',1,20,threshold);
        figure(3);
        imshow(Rv_C/1023 .* Rv_goodpixels);
        
        [Rh_C, Rh_goodpixels] = decode('right/right_',21,40,threshold);
        figure(4);
        imshow(Rh_C/1023 .* Rh_goodpixels);
        
        % combine the horizontal and vertical coordinates into a single (20 bit, assuming 10 bit resolution) code in [0...1048575].
        R_C = Rh_C + bitshift(Rv_C, 10);
        L_C = Lh_C + bitshift(Lv_C, 10);
        
        % identify pixels that have both good horiztonal and vertical codes.
        R_goodpixels = Rh_goodpixels & Rv_goodpixels; 
        L_goodpixels = Lh_goodpixels & Lv_goodpixels;
        
        % find the indicies of pixels that were succesfully decoded.
        R_sub = find(R_goodpixels);
        L_sub = find(L_goodpixels);
        
        % pull out the codes for the good pixels.
        R_C_good = R_C(R_sub);          
        L_C_good = L_C(L_sub);

        %intersect the codes of good pixels in the left and right image to find matches.
        [matched, iR, iL] = intersect(R_C_good, L_C_good); 
        
        % get the pixel indicies of the pixels that were matched.
        R_sub_matched = R_sub(iR);  
        L_sub_matched = L_sub(iL);
        
        % create arrays containing the pixel coordinates.
        h = size(Lv_C,1);
        w = size(Lv_C,2);
        [xx,yy] = meshgrid(1:w,1:h); 
        
        % pull out the x,y coordinates of the matched pixels.
        xR(1,:) = xx(R_sub_matched); 
        xR(2,:) = yy(R_sub_matched); 
        xL(1,:) = xx(L_sub_matched); 
        xL(2,:) = yy(L_sub_matched);
        
        % load in the callibration.
        load 'scan0_calibration.mat';
        
        % convert to 3d points.
        X = triangulate(xL, xR, camL, camR);
        
        save 'X.mat' X;
        save 'xL.mat' xL;
        save 'xR.mat' xR;
end