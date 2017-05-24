%
% This script tests the project and triangulate functions.
% You may want to modify it to try other tests and/or cut
% and paste bits into your interactive MATLAB session as 
% you are debugging.
%


%
% first generate our test figure in 3D
%
X = generate_hemisphere(2,[0;0;10],1000);

%
% set intrinsic parameters shared by both camers
%

%focal length
camL.f = 100;
camR.f = 100;


%location of camera center
camL.c = [50;50];
camR.c = [50;50];

%
% extrinsic params for left camera
%
camL.t= [-0.2;0;0];
thy = atan2(camL.t(1),10); 
Ry = [  cos(thy)   0  -sin(thy) ; ...
              0    1         0 ; ...
       sin(thy)   0  cos(thy) ];
camL.R = Ry;

%
%extrinsic params for right camera
%
camR.t= [0.2;0;0];
thy = atan2(camR.t(1),10); 
Ry = [  cos(thy)   0  -sin(thy) ; ...
              0    1         0 ; ...
       sin(thy)   0  cos(thy) ];
camR.R = Ry;

%
% now compute the two projections
%
xL = project(X,camL);
xR = project(X,camR);

%
% now try to recover the 3D locations from the 2D views
%
% uncomment these lines to add some noise to the point locations
% in order to make the process more realistic.
%
%xL = xL + 0.01*randn(size(xL)); 
%xR = xR + 0.01*randn(size(xR));
%

Xrecov = triangulate(xL,xR,camL,camR);

%
% display results as a cloud of points
%
figure(3); clf;
plot3(X(1,:),X(2,:),X(3,:),'.');
hold on;
plot3(Xrecov(1,:),Xrecov(2,:),Xrecov(3,:),'ro');
axis image;
axis vis3d;
grid on;
legend('original points','recovered points')

%
%display results as a surface
%
figure(4);
subplot(2,1,1);
tri = delaunay(X(1,:),X(2,:));
trisurf(tri,X(1,:),X(2,:),X(3,:));
axis equal; axis vis3d; grid on;
title('original shape');
subplot(2,1,2);
tri = delaunay(Xrecov(1,:),Xrecov(2,:));
trisurf(tri,Xrecov(1,:),Xrecov(2,:),Xrecov(3,:));
axis equal; axis vis3d; grid on;
title('recovered shape');



%
% now mess up our left camera's focal length and compute the two projections
%
camL.f = 110;
xL = project(X,camL);
xR = project(X,camR);

%
% now try to recover the 3D locations from the 2D views
%
% uncomment these lines to add some noise to the point locations
% in order to make the process more realistic.
%
%xL = xL + 0.01*randn(size(xL)); 
%xR = xR + 0.01*randn(size(xR));
%

Xrecov = triangulate(xL,xR,camL,camR);

%
% display results as a cloud of points
%
figure(5); clf;
plot3(X(1,:),X(2,:),X(3,:),'.');
hold on;
plot3(Xrecov(1,:),Xrecov(2,:),Xrecov(3,:),'ro');
axis image;
axis vis3d;
grid on;
legend('original points','recovered points with incorrect focal length')

%
%display results as a surface
%
figure(6);
subplot(2,1,1);
tri = delaunay(X(1,:),X(2,:));
trisurf(tri,X(1,:),X(2,:),X(3,:));
axis equal; axis vis3d; grid on;
title('original shape');
subplot(2,1,2);
tri = delaunay(Xrecov(1,:),Xrecov(2,:));
trisurf(tri,Xrecov(1,:),Xrecov(2,:),Xrecov(3,:));
axis equal; axis vis3d; grid on;
title('recovered shape with incorrect focal length');
