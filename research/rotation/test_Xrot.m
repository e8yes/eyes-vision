function test_Xrot()
          X = 10*rand(2,50);              % generate 50 random points in 2D
         figure(1); clf;                 % display a figure and clear it.
         plot(X(1,:),X(2,:),'g.');       % plot the original points as green dots
         Xrot = rotate(X,45);            % rotate the points by 45 degrees counterclockwise
         hold on;                        % tell matlab to not clear the figure when plotting additional points
         plot(Xrot(1,:),Xrot(2,:),'r.'); % plot the rotated points on the same figure
end