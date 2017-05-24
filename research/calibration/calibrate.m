function [cam,Pcam,Pworld] = calibrate(imfile, left);
        % function [cam,Pcam,Pworld] = calibrate(imfile);
        %
        %  This function takes an image file name, loads in the image
        %  and uses it for camera calibration.  The user clicks
        %  on corner points of a grid (assumed to be a particular calibration
        %  object).  These  points along with their true coordinates are used to 
        %  optimize the camera parameters with respect to the reprojection
        %  error.
        %
        %  Output: 
        %     cam : a data structure describing the recovered camera
        %     Pcam : the 2D coordinates of points in the image
        %     Pworld : the 3D "ground truth" coordinates of the points in the image
        %
        I = im2double(rgb2gray(imread(imfile)));

        % get the grid corner points
        fprintf('click on the corners of each of the three faces\n');
        fprintf('always start at the origin point and go around in a circle\n');
        fprintf('XY plane  (9x8 squares) -> go along the 9 edge first  (CCW)\n');
        XY = mapgrid(I,9,8);  
        fprintf('XZ plane  (10x8 squares) -> go along the 10 edge first (CW)\n');
        XZ = mapgrid(I,10,8);
        fprintf('YZ plane  (8x10 squares) -> go along the 8 edge first (CCW)\n');
        YZ = mapgrid(I,8,10);


        % true 3D cooridnates (in cm) of the grid corners. this is only correct 
        % assuming points were clicked in the order specified above
        [yy,xx] = meshgrid( linspace(0,19.55,8), linspace(0,22.05,9));
        zz = zeros(size(yy(:)));
        XYworld = [xx(:) yy(:) zz(:)]';

        [zz,xx] = meshgrid(linspace(0,19.55,8),linspace(0,25.2,10));
        yy = zeros(size(xx(:)));
        XZworld = [xx(:) yy(:) zz(:)]';

        [zz,yy] = meshgrid(linspace(0,25.2,10),linspace(0,19.55,8));
        xx = zeros(size(yy(:)));
        YZworld = [xx(:) yy(:) zz(:)]';


        % put all the points from the calibration object into a single array
        % NOTE: the order here has to match up so that the nth point in Pcam
        % and the nth point in Pworld correspond.
        Pcam = [XY XZ YZ];
        Pworld = [XYworld XZworld YZworld];


        % assume the principal point is in the center of the image
        cy = size(I,1) / 2;
        cx = size(I,2) / 2;

        % initial guesses of other parameters.
        f = 1000;
        thx = 3*pi/2; thy = 3*pi/4; thz = 0; %no rotation
        tx = 100; ty = 100; tz = 100; %make sure camera is translated away from origin

        % initial parameter vector  
        % order needs to match whatever you use 
        % in your project_error function)
        paramsinit = [f; thx; thy; thz; tx; ty; tz];  

        % setup the optimization routine 
        opts = optimset('maxfunevals',100000,'maxiter',10000);  %set the max number of iterations

        % use an anonymous function to capture the fixed parameters: Pword,Pcam,cx and cy.  
        %  The remaining parameters are optimized over
        params_opt = lsqnonlin( @(params) project_error(params,Pworld,Pcam,cx,cy),paramsinit,[],[],opts);

        % now unpack params_opt vector back into a cam struct.
        cam.f = params_opt(1);
        Rx = [1      0                       0; 
              0      cos(params_opt(2))      -sin(params_opt(2));
              0      sin(params_opt(2))      cos(params_opt(2))];
        
        Ry = [cos(params_opt(3))            0       sin(params_opt(3));
              0                             1       0;
              -sin(params_opt(3))           0       cos(params_opt(3))];
        
        Rz = [cos(params_opt(4))            -sin(params_opt(4))     0
              sin(params_opt(4))            cos(params_opt(4))      0
              0                             0                       1];
        cam.R = Rz*Ry*Rx;
        cam.t = [params_opt(5); params_opt(6); params_opt(7)];
        cam.c = [cx; cy];
        

        if (1)
            % here is a nice visualization.... plot projected locations of the 3D
            %  points on top of the 2D image so we can visualize the reprojection error
            Pest = project(Pworld,cam);
            figure(3); clf;
            imagesc(I); axis image; colormap gray
            hold on;
            plot(Pcam(1,:),Pcam(2,:),'b.')
            plot(Pest(1,:),Pest(2,:),'r.')
            hold off;
            title('reprojections after optimization');
        end
end

