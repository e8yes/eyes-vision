function pts = inputpts(n)
        pts = [];
        hold on
        for i=1:n
                [x, y] = ginput(1);
                plot(x, y, 'r');
                pts = [pts; x y];
        end
        hold off
end