
function err = project_error(params,X,xL,cx,cy)
        % function [x] = project_error(params,X,x,cx,cy)
        %
        %
        % Input:
        %
        % params : an 7x1 vector containing the camera parameters which we 
        %   will optimize over.  this should include:
        % 
        %    f - the focal length 
        %    thx,thy,thz - camera rotation around the x,y and z axis
        %    tx,ty,tz - camera translation vector entries
        %   
        %    cx,cy - the camera center.  we will just assume this is the 
        %      center of the image so we won't optimize over it  (it is not part of params)
        %
        % X: a 3xN matrix containing the point coordinates in 3D world coordinates (in physical units, e.g. cm)
        %
        % x: a 2xN matrix containing the point coordinates in the camera image (pixels)
        %
        % Output:
        %
        %  err : a 2xN matrix containing the difference between x and project(X,cam)
        %
        %

        % unpack parameters and build up cam data structure needed by the project function
        cam.c = [cx; cy];
        cam.f = params(1);
        Rx = [1      0               0; 
              0      cos(params(2))  -sin(params(2));
              0      sin(params(2))  cos(params(2))];
              
        Ry = [cos(params(3))         0      -sin(params(3));
              0                      1       0;
              sin(params(3))        0       cos(params(3))];
              
        Rz = [cos(params(4))         -sin(params(4))  0;
              sin(params(4))         cos(params(4))   0;
              0                      0                1];
        cam.R = Rx*Rz*Ry;
        cam.t = [params(5); params(6); params(7)];

        % compute projection
        xproject = project(X,cam);

        % compute the vector of reprojection errors
        err = xL - xproject;
end

