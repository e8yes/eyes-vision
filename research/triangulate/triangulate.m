function X = triangulate(xL,xR,camL,camR)
%
%  function X = triangulate(xL,xR,camL,camR)
%
%  INPUT:
%   
%   xL,xR : points in left and right images  (2xN arrays)
%   camL,camR : left and right camera parameters
%
%
%  OUTPUT:
%
%    X : 3D coordinate of points in world coordinates (3xN array)
%
%
% 1. convert xL and xR from pixel coordinates back into meters with unit focal 
% length by subtracting off principal point and dividing through by 
% focal length...  call the results qR and qL.
        qL = (xL - camL.c)./camL.f;
        qR = (xR - camR.c)./camL.f;

% 2. make the right camera the origin of the world coordinate system by 
% transforming both cameras appropriately in order to find the rotation
% and translation (R,t) relating the two cameras
        cam_R = camL.R * camR.R';
        cam_t = camR.R' * (camL.t - camR.t);
        
% 3. Loop over each pair of corresponding points qL,qR and 
% solve the equation:  
%
%   Z_R * qR = Z_L * R * qL + t
%
% for the depth values Z_R and Z_L using least squares.
%
        qL = [qL; ones(size(qL,2),1)'];
        qR = [qR; ones(size(qR,2),1)'];
        
        Z_R = [];
        for i=1:size(qL, 2)
                qL_rot = cam_R * qL(:,i);
                qR_h = qR(:,i);
                A = [qR_h -qL_rot];
                % argmin_Z { ||A*Z - t||^2 }
                Z = inv(A' * A) * A' * cam_t;
                Z_R = [Z_R [Z(1); Z(1); Z(1)]];
        end

% 4. use Z_R to compute the 3D coordinates XR = (X_R,Y_R,Z_R) in right camera
% reference frame
        XR = qR.*Z_R;

% 5. since the right camera wasn't at the origin, map XR back to world coordinates 
% X using the right camera transformation parameters.
        X = camR.R * XR + camR.t;
end


