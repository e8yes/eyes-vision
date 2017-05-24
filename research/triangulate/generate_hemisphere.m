
function X = generate_hemisphere(radius,center,npts,visualize)

%
% function X = generate_hemisphere(radius,center,npts,visualize)
% 
%  generate 3D points coordinates on the surface of a hemisphere
%  of a given radius and location.
%
%  INPUTS:
%
%    radius : radius of the hemisphere  (scalar)
%    center : center point of the hemisphere  (3x1 vector)
%    npts : number of points to put on the hemisphere (scalar)
%    visualize : (optional argument) if visualize=true, pop up a 
%        new figure and visualize the points
%
%  OUTPUTS:
%
%    X : a 3 x npts array containing 3D coordinates of the points
%        

if (nargin < 4)
  visualize = false;
end

if (nargin < 3)
  help genpoints
  error('too few arguments')
end

% first generate random gaussian distributed points
X = randn(3,npts);   

% now map those points to the surface of a sphere by
% making them all unit norm (scale each vector by its
% norm)
nx = sqrt(sum(X.^2));
X = radius * X ./ repmat(nx,3,1);

% now make those points with positive z coordinates
% have negative zcoordintes in order to make it a 
% hemisphere instead of a sphere.
X(3,:) = -abs(X(3,:));


% now translate the points to center
X = X + repmat(center,1,npts);

if (visualize)
  figure;
  subplot(2,1,1);
  plot3(X(1,:),X(2,:),X(3,:),'.')
  axis equal; axis vis3d; grid on;
  title('3D sampled points');

  subplot(2,1,2);
  tri = delaunay(X(1,:),X(2,:));
  trisurf(tri,X(1,:),X(2,:),X(3,:));
  shading interp
  axis equal; axis vis3d; grid on;
  title('hemisphere');
end
