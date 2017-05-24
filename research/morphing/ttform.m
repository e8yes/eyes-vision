function T = ttform(tri1,tri2)
% compute the affine transformation T that maps points
% of triangle1 to triangle2 
%
%  tri1 : 2x3 matrix containing coordinates of corners of triangle 1
%  tri2 : 2x3 matrix containing coordinates of corners of triangle 2
%
%  T : the resulting transformation, should be a 3x3
%      matrix which operates on points described in 
%      homogeneous coordinates 
%
          
        %
        % your code here should figure out the matrix T
        %
        A = [
                tri1(1,1) tri1(2,1) 1 0 0 0;
                0 0 0 tri1(1,1) tri1(2,1) 1;
                
                tri1(1,2) tri1(2,2) 1 0 0 0;
                0 0 0 tri1(1,2) tri1(2,2) 1;
                
                tri1(1,3) tri1(2,3) 1 0 0 0;
                0 0 0 tri1(1,3) tri1(2,3) 1
        ];
        
        b = [
                tri2(1,1); tri2(2,1);
                tri2(1,2); tri2(2,2);
                tri2(1,3); tri2(2,3);
        ];

        S = inv(A)*b;
        
        T = [
                S(1,1) S(2,1) S(3,1);
                S(4,1) S(5,1) S(6,1);
                0 0 1;
        ];


        %
        % test code to make sure we have done the right thing
        % if we apply the transformation T to tri1, we should
        % get exactly tri2.
        %

        % apply mapping to corners of tri1 and 
        % make sure the result is close to tri2
        err1 = sum((T*[tri1(:,1);1] - [tri2(:,1);1]).^2);
        assert(err1 < eps)

        err2 = sum((T*[tri1(:,2);1] - [tri2(:,2);1]).^2);
        assert(err2 < eps)

        err3 = sum((T*[tri1(:,3);1] - [tri2(:,3);1]).^2);
        assert(err3 < eps)
end
