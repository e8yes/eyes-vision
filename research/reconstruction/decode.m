function [C,goodpixels] = decode(imageprefix,start,stop,threshold)
% function [C,goodpixels] = decode(imageprefix,start,stop,threshold)
%
%
% Input:
%
% imageprefix : a string which is the prefix common to all the images.
%
%                  for example, pass in the prefix '/home/fowlkes/left/left_'  
%                  to load the image sequence   '/home/fowlkes/left/left_01.jpg' 
%                                               '/home/fowlkes/left/left_02.jpg'
%                                                          etc.
%
%  start : the first image # to load
%  stop  : the last image # to load
% 
%  threshold : the pixel brightness should vary more than this threshold between the positive
%             and negative images.  if the absolute difference doesn't exceed this value, the 
%             pixel is marked as undecodeable.
%
% Output:
%
%  C : an array containing the decoded values (0..1023) 
%
%  goodpixels : a binary image in which pixels that were decodedable across all images are marked with a 1.
        seq = {};
        for i=start:stop
                if i < 10
                        suffix = strcat(strcat('0',num2str(i)),'.jpg');
                else
                        suffix = strcat(num2str(i),'.jpg');
                end
                seq{i-start+1} = rgb2gray(im2double(imread(strcat(imageprefix,suffix))));
        end
        
        C = zeros(size(seq{1}));
        
        % all good.
        goodpixels = ones(size(seq{1}));
        
        % gray code.
        G = zeros(size(seq{1}));
        
        nbits = (stop-start+1)/2;
        
        % decode gray code.
        for i=1:2:stop-start+1
                D = seq{i} - seq{i+1};
                
                % current valid pixels.
                P = abs(D) > threshold;
                
                % valid pixels currently.
                goodpixels(:) = bitand(P(:), goodpixels(:));
                
                % decode.
                mask = zeros(size(D));
                mask(D > 0) = 1;
                mask = bitshift(mask, nbits-floor(i/2)-1);
                G = bitor(G, mask);
        end
        
        % convert to binary.
        C = zeros(size(seq{1}));
        % msb copy over.
        m = bitshift(1,nbits-1);
        C = bitor(C,bitand(G,m));
        for k=nbits-1:-1:0
                m = bitshift(i,k);
                m2 = bitshift(i,k-1);
                L = bitshift(bitand(C,m),-1);
                p = bitxor(L,bitand(G,m2));
                C = bitor(C,p);
        end
end