load 'xL.mat';
load 'xR.mat';
load 'X.mat';

figure(1);
scatter(xL(1,:),xL(2,:));

figure(2);
scatter(xL(1,:),xL(2,:));

figure(3);
axis([-10 50 -10 50 -10 50]);
scatter3(X(1,:),X(2,:),X(3,:));