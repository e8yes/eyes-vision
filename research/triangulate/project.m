function [x] = project(X,cam);

% function [x] = project(X,cam)
%
% Carry out projection of 3D points into 2D given the camera parameters
% We assume that the camera with the given intrinsic parameters produces
% images by projecting onto a focal plane at distance cam.f along the 
% z-axis of the camera coordinate system.
%
% Our convention is that the camera starts out the origin (in world
% coordinates), pointing along the z-axis.  It is first rotated by 
% some matrix cam.R and then translated by the vector cam.t.
%
%
% Input:
%
%  X : a 3xN matrix containing the point coordinates in 3D world coordinates (meters)
%
%  intrinsic parameters:
%
%  cam.f : focal length (scalar)
%  cam.c : image center (principal point) [in pixels]  (2x1 vector)
%
%  extrinsic parameters:
%
%  cam.R : camera rotation matrix (3x3 matrix)
%  cam.t : camera translation matrix (3x1 vector)
%
%
% Output:
%
%  x : a 2xN matrix containing the point coordinates in the 2D image (pixels)
%



% 1. transform the points in the world to the camera coordinate frame
        T = [[cam.R'; 0 0 0] [0 0 0 1]']*[[1 0 0; 0 1 0; 0 0 1; 0 0 0] [-cam.t; 1]];
        cam_x = T * [X; ones(size(X, 2), 1)'];
% 2. check to see which points are in front of the camera and print
% a message to the console indicating how many points are in front
% of the camera... this can be useful for debugging later on.
        front_x = find(cam_x(3:3,:) > 0);
        strcat(strcat('There are _ ', num2str(size(front_x,2))), ' points in front of the camera')
        
% 3. project the points down onto the image plane and scale by focal length
        proj_x = cam_x(1:2,:) ./ cam_x(3:3,:) .* cam.f;

% 4. add in camera principal point offset to get pixel coordinates
        x = proj_x + cam.c;
end

