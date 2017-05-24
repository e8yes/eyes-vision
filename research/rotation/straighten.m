I = im2double(imread("img/crooked_horizon.jpg"));

figure(1);
imshow(I);

hold on;
[x0, y0] = ginput(1);
[x1, y1] = ginput(1);

plot(p0, p1, 'r');
hold off;

v0 = [x0 y0] - [x1 y1];
v0 = v0/norm(v0);
vx = [1 0];
angle = pi - acos(dot(v0,vx));

R = rotate_image(I(:,:,1), -angle);
G = rotate_image(I(:,:,2), -angle);
B = rotate_image(I(:,:,3), -angle);

figure(2);
imshow(cat(3,R,G,B));