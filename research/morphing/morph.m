
%
% morphing script
%

% load in two images...
I1 = im2double(imread('img/c1.jpg'));
Itmp = im2double(imread('img/c2.jpg'));
I2 = imresize(Itmp,[size(I1,1) size(I1,2)],'bilinear');

% get user clicks on keypoints using either ginput or cpselect tool
%figure(1);
%imshow(I1);
%P1 = inputpts(30);
%save 'P1.mat' P1

%figure(2);
%imshow(Itmp);
%P2 = inputpts(30);
%save 'P2.mat' P2

load 'P1.mat'
load 'P2.mat'


P2(:,1) = P2(:,1)*size(I2,2)/size(Itmp,2);
P2(:,2) = P2(:,2)*size(I2,1)/size(Itmp,1);

pts_img1 = transpose(P1);
pts_img2 = transpose(P2);

% append the corners of the image to your list of points
% this assumes both images are the same size and that your
% points are in a 2xN array
[h,w,~] = size(I1);
pts_img1 = [pts_img1 [0,0]' [w,0]' [0,h]' [w,h]'];
pts_img2 = [pts_img2 [0,0]' [w,0]' [0,h]' [w,h]'];

% generate triangulation 
pts_halfway = 0.5*pts_img1 + 0.5*pts_img2;
tri = delaunay(pts_halfway(1,:),pts_halfway(2,:));
%hold on, triplot(tri,pts_halfway(1,:),pts_halfway(2,:)), hold off


% now produce the frames of the morph sequence
nframe = 5
for fnum = 1:nframe
        t = (fnum-1)/(nframe-1);
        
        % intermediate key-point locations
        pts_target = (1-t)*pts_img1 + t*pts_img2;                

        % warp both images towards target
        I1_warp = warp(I1,pts_img1,pts_target,tri);              
        I2_warp = warp(I2,pts_img2,pts_target,tri);                                            

        % blend the two warped images
        Iresult = (1-t)*I1_warp + t*I2_warp;                     

        % display frames
        figure(1); clf; imagesc(Iresult); axis image; drawnow;   

        % alternately save them to a file to put in your writeup
        imwrite(Iresult,sprintf('frame_%2.2d.jpg',fnum),'jpg');   
end
