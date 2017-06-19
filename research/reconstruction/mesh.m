function mesh()
        load 'X.mat';
        load 'xL.mat';
        
        % pruning.
        bbox = [30; 30; 30];
        center = sum(X,2)/size(X,2);
        bmin = center - bbox/2;
        bmax = center + bbox/2;
        
        a = find((X(1,:) > bmin(1)) & (X(1,:) < bmax(1)));
        b = find((X(2,:) > bmin(2)) & (X(2,:) < bmax(2)));
        c = find((X(3,:) > bmin(3)) & (X(3,:) < bmax(3)));
        
        valid = intersect(c,intersect(a,b));
        
        X_bounded = X(:,valid);
        tri = delaunay(xL(1,valid),xL(2,valid));
        
        % remove long edge.
        e1 = X_bounded(:,tri(:,1)) - X_bounded(:,tri(:,2));
        e2 = X_bounded(:,tri(:,2)) - X_bounded(:,tri(:,3));
        e3 = X_bounded(:,tri(:,3)) - X_bounded(:,tri(:,1));
        edist = [dot(e1,e1); dot(e2,e2); dot(e3,e3)];
        
        len_threshold = 1*1;
        good_tri = find((edist(1,:) < len_threshold) & (edist(2,:) < len_threshold) & (edist(3,:) < len_threshold));
        tri_clean = tri(good_tri,:);
        
        % remove isolated points and update indices.
        connected = union(tri_clean(:,3), union(tri_clean(:,1), tri_clean(:,2)));
        X_connected = X_bounded(:,connected);
        old2new = zeros(size(X,2),1);
        old2new(connected) = 1:length(connected);
        tri_final = [old2new(tri_clean(:,1))    old2new(tri_clean(:,2))         old2new(tri_clean(:,3))];
        
        figure(1); clf;
        h = trisurf(tri_final,X_connected(1,:),X_connected(2,:),X_connected(3,:));
        set(h,'edgecolor','none');
        set(gca,'projection','perspective');
        set(gcf,'renderer','opengl');
        axis image; axis vis3d;
        camorbit(120,0); camlight left;
        camorbit(120,0); camlight left;
        lighting phong;
        material dull;
end