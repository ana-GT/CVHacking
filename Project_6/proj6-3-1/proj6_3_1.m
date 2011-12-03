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

testData = zeros( n, 1 + size(testA,2) );

testData( 1:nA, 3 ) = 1*ones(nA, 1); % A
testData( nA+1:end, 3 ) = 2*ones(nB, 1); %B

testData(:, 1:2 ) = [ testA; testB ];


% Find limits
x1_min = min( testData(:,1) )
x2_min = min( testData(:,2) )
x1_max = max( testData(:,1) )
x2_max = max( testData(:,2) )

k = 100;
dx1 = (x1_max - x1_min)/k
dx2 = (x2_max - x2_min)/k

% Find best x1 slump

x_slump = zeros(k,2);
error = zeros(k,2);

x_slump(1,:) = [x1_min, x2_min];

for i = 1:k

	predicted_x = zeros(n,2);

	% Classify
	for j = 1:n
		% x1
    	if testData(j,1) < x_slump(i,1)
		  predicted_x(j,1) = 1; 
		else
		  predicted_x(j,1) = 2;
		endif
		% x2
    	if testData(j,2) < x_slump(i,2)
		  predicted_x(j,2) = 1; 
		else
		  predicted_x(j,2) = 2;
		endif

	end 	

	% Check accuracy
    count1 = 0; count2 = 0;

	for j = 1:n
    	if predicted_x(j,1) != testData(j,3)
			count1++;
		endif

    	if predicted_x(j,2) != testData(j,3)
			count2++;
		endif

	end 	
	error(i,1) = count1;
    error(i,2) = count2;   

	% Update slump
	x_slump(i+1,1) = x_slump(i,1) + dx1;
    x_slump(i+1,2) = x_slump(i,2) + dx2;	
end

[minx1, minloc1] = min(error(:,1) );
[minx2, minloc2] = min(error(:,2) );

x1Slump = x_slump(minloc1,1)
x2Slump = x_slump(minloc2,2)

printf("Slump x1: %f error: %d for %d training data \n", x1Slump, error(minloc1, 1), n);
printf("Slump x2: %f error: %d for %d training data \n", x2Slump, error(minloc2, 2), n);

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


