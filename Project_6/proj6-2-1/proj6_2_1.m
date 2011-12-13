%% proj_6_2_1.m
%% Author: A. Huaman
%% Date: 2012/12/03

%% No,sir, I did not surrender to MatLab, this is Octave. Long live to Open Source
%% PS.- The only reason why I am not using C++ this time is because well...gnuplot is
%% easier to call from Octave. Pure laziness.


% Load train data
trainA = load("trainA_blank.txt");
trainB = load("trainB_blank.txt");

% Load test data
testA = load("testA_blank.txt");
testB = load("testB_blank.txt");

nA = size( trainA, 1 );
nB = size( trainB, 1 );

% Putting together the test stuff
n = nA + nB;

trainData = zeros( n, 2 + size(trainA,2) );

trainData( 1:nA, 3 ) = 1*ones(nA, 1); % A
trainData( nA+1:end, 3 ) = -1*ones(nB, 1); %B
trainData( :, 4 ) = 1; % A

trainData(:, 1:2 ) = [ trainA; trainB ];


[stumps, err, hs] = xStumps( trainData );

err1 = err(1,1); err2 = err(1,2);
err3 = err(1,3); err4 = err(1,4);

e1Slump = 0;
e2Slump = 0;

if( err1 < err2)
	x1Slump = stumps(1,1);
    e1Slump = err1;
else
	x1Slump = stumps(1,2);
    e1Slump = err2;
endif

if( err3 < err4)
	x2Slump = stumps(1,3);
	e2Slump = err3;
else
	x2Slump = stumps(1,4);
    e2Slump = err4;
endif


printf("Slump x1: %f error: %d for %d training data \n", x1Slump, e1Slump, n);
printf("Slump x2: %f error: %d for %d training data \n", x2Slump, e2Slump, n);

% Plot train set
figure(1);
plot( trainA(:,1), trainA(:,2), 'mx', 'markersize', 5, 'linewidth', 5 );
hold on;
plot( trainB(:,1), trainB(:,2), 'bx', 'markersize', 5, 'linewidth', 5  );
plot( [x1Slump, x1Slump], ylim, 'g', 'linewidth', 5  );
plot( xlim, [x2Slump, x2Slump], 'y', 'linewidth', 5  );
legend('Class A', 'Class B','x1 stump', 'x2 stump' );
title('Train Set');
print('proj6-2-1-Slump.png', '-dpng');
hold off;


