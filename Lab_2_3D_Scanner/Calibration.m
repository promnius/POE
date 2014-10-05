
distances = [20  25  30  35  40  45  50  55  60  65  70  75  80  85  90  95  100 105 110 115 120 125 130 135 140 145 150];
outputs =   [515 462 399 346 299 268 240 217 197 178 166 157 145 138 128 120 112 105 100 92  88  84  80  75  72  67  64 ];
%plot(outputs,distances)
RadPan = (pan.* pi) ./ 180;
RadTilt = (tilt .* pi) ./ 180;
for i=0:((length(distance)/10)-1)
    refinedDistance(i+1) = mean(distance(10*i+1:10*i+10));
end
for i=0:((length(pan)/10)-1)
    refinedPan(i+1) = mean(pan(10*i+1:10*i+10));
end
for i=0:((length(tilt)/10)-1)
    refinedTilt(i+1) = mean(tilt(10*i+1:10*i+10));
end

% figure
% x = refinedDistance.*sind(refinedPan).*cosd(refinedTilt);
% y = refinedDistance.*sind(refinedPan).*sind(refinedTilt);
% z = refinedDistance.*cosd(refinedPan);

% x = distance.*sind(pan).*cosd(tilt);
% y = distance.*sind(pan).*sind(tilt);
% z = distance.*cosd(pan);

%plot3(refinedPan,refinedTilt,refinedDistance)
c = 1;
for p=1:10
    for t=1:273
        newMatrix(p,t) = distance(c);
        c = c + 1;
    end
end
smallerPan = pan(1:10*273)
smallerTilt = tilt(1:10*273)
pcolor(newMatrix)
%figure
%[X,Y,Z] = sph2cart(RadPan,RadTilt,distance);
% plot3(x,y,z,'.b')
% axis([-50 50 0 50 0 50])
