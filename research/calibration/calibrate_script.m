%
% simple script to test calibration for a stereo pair of images
%


%
% do calibration for the two cameras
%
[camL,xL,Xtrue] = calibrate('left_calib.jpg');
[camR,xR,Xtrue] = calibrate('right_calib.jpg');

% now do triangulation to reconstruct the calibration object from our cameras
Xest = triangulate(xL,xR,camL,camR);

% plot the triangulated point locations
figure(1); clf;
hold on;
plot3(Xest(1,:),Xest(2,:),Xest(3,:),'r.')
plot3(Xtrue(1,:),Xtrue(2,:),Xtrue(3,:),'.')

% for question 2 you want to compare Xest and Xtrue numerically

% visualize the recovered camera locations in the same plot
% by drawing a vector from the camera origin along the 
% z axis.
unitz = [0 0 5]';

lcamo = camL.t; lcamz = camL.t + camL.R*unitz;
plot3([lcamo(1) lcamz(1)],[lcamo(2) lcamz(2)],[lcamo(3) lcamz(3)],'g-','LineWidth',3);
rcamo = camR.t; rcamz = camR.t + camR.R*unitz;
plot3([rcamo(1) rcamz(1)],[rcamo(2) rcamz(2)],[rcamo(3) rcamz(3)],'c-','LineWidth',3);
plot3(lcamo(1),lcamo(2),lcamo(3),'gs');
plot3(rcamo(1),rcamo(2),rcamo(3),'cs');

axis vis3d; axis image;
grid on;
xlabel('X'); ylabel('Y'); zlabel('Z');

set(gca,'projection','perspective')
legend('estimate','true','left camera','right camera')





