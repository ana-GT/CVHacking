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

% Assigning the test set (+) and (-)
n = nA + nB;

% 1: x1
% 2: x2 
% 3: label 
% 4: weight
testData = zeros( n, 2 + size(testA,2) );

% labels
testData( 1:nA, 3 ) = 1*ones(nA, 1); % A : +1
testData( nA+1:end, 3 ) = -1*ones(nB, 1); %B : -1

% x1, x2
testData(:, 1:2 ) = [ testA; testB ];

%weights
testData(:, 4) = (1/n)*ones(n,1);


% For each training stage
T = 4;

for j = 1:T

	% a. Normalize weights
	Sw = sum( testData(:,4) );
	newWeights = testData(:,4)/Sw;
	testData(:,4) = newWeights;

	% b. Select the best classifier
	[x1_stump, err1, h1] = x1Stump( testData );
	[x2_stump, err2, h2] = x2Stump( testData );	

    if( err1 < err2 )
      weakClass(j,:) = [x1_stump, 1];
      e_T = err1; h_T = h1;
    else
      weakClass(j,:) = [x2_stump, 2];
	  e_T = err2; h_T = h2;
	endif

    % Check
	if( e_T >= 1/2)
		break;
	endif

	alpha_T = (1/2)*log((1-e_T)/e_T); 

	% Update
	newWeights = (testData(:,4).*exp(-alpha_T.*testData(:,3).*h_T) );
	Sw = sum( newWeights );
    newWeights = newWeights/Sw;
	testData(:,4) = newWeights;	
end

weakClass

printf("Done \n");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [x1Slump, err, h1] = x1Stump( data )

	n = size(data,1);

	% Find limits
	x1_min = min( data(:,1) );
	x1_max = max( data(:,1) );

	k = 100;
	dx1 = (x1_max - x1_min)/k;

	% Find best x1 slump

	x1_slump = zeros(k,1);
	error1 = zeros(k,1);

	x1_slump(1,1) = x1_min;

	for i = 1:k

		predicted_x = zeros(n,1);

		% Classify
		for j = 1:n
			% x1
    		if data(j,1) < x1_slump(i,1)
		  		predicted_x(j,1) = 1; % A 
			else
		  		predicted_x(j,1) = -1; % B
			endif
		end 	

		% Check accuracy
    	count1 = 0;

		for j = 1:n
    		if predicted_x(j,1) != data(j,3)
				count1 = count1 + data(j,4);
			endif
		end 	
		error1(i,1) = count1;

		% Update slump
		x1_slump(i+1,1) = x1_slump(i,1) + dx1;
	end

	[minx1, minloc1] = min(error1(:,1) );

	x1Slump = x1_slump(minloc1,1);
    err = error1(minloc1,1);

	% h
	h1 = zeros(n,1);
	for j = 1:n
   		if data(j,1) < x1Slump
	  		h1(j,1) = 1; % A 
		else
	  		h1(j,1) = -1; % B
		endif
	end 	

endfunction

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [x2Slump, err, h2] = x2Stump( data )

	n = size(data,1);

	% Find limits
	x2_min = min( data(:,2) );
	x2_max = max( data(:,2) );

	k = 100;
	dx2 = (x2_max - x2_min)/k;

	% Find best x1 slump

	x2_slump = zeros(k,1);
	error2 = zeros(k,1);

	x2_slump(1,1) = x2_min;

	for i = 1:k

		predicted_x = zeros(n,1);

		% Classify
		for j = 1:n
			% x1
    		if data(j,2) < x2_slump(i,1)
		  		predicted_x(j,1) = 1; % A 
			else
		  		predicted_x(j,1) = -1; % B
			endif
		end 	

		% Check accuracy
    	count2 = 0;

		for j = 1:n
    		if predicted_x(j,1) != data(j,3)
				count2 = count2 + data(j,4);
			endif
		end 	
		error2(i,1) = count2;

		% Update slump
		x2_slump(i+1,1) = x2_slump(i,1) + dx2;
	end

	[minx2, minloc2] = min(error2(:,1) );

	x2Slump = x2_slump(minloc2,1);
    err = error2(minloc2,1);

	% h
	h2 = zeros(n,1);
	for j = 1:n
   		if data(j,2) < x2Slump
	  		h2(j,1) = 1; % A 
		else
	  		h2(j,1) = -1; % B
		endif
	end 	

endfunction

 
