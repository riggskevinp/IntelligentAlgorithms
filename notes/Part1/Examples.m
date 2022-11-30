x = 0:0.5:100;
x2 = -5:0.1:5;

y1 = 1 ./ (1 + ((x-50)/10).^4);

y2 = exp(-x2.^2);

subplot(211)
plot(x,y1)

subplot(212)
plot(x2,y2)