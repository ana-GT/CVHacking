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
printf("Training data: A: %d B: %d - total: %d \n", nA, nB, n);

% 1: x1
% 2: x2 
% 3: label 
% 4: weight
trainData = zeros( n, 4 );

% x1, x2
trainData(:, 1:2 ) = [ trainA; trainB ];

% labels
trainData( 1:nA, 3 ) = 1*ones(nA, 1); % A : +1
trainData( nA+1:end, 3 ) = -1*ones(nB, 1); %B : -1

%weights
trainData(:, 4) = (1/n).*ones(n,1);


% For each training stage
T = 16;

weakClass = zeros(T,2);
e_T = 0; h_T = zeros(n,1);

for j = 1:T

	%  Select the best classifier

    [stumps, err, hs] = xStumps( trainData );

    ht = abs( 0.5*ones(1,4) - err );

    [maxval, maxloc] = max(ht);
    weakClass(j,:) = [stumps(1,maxloc), maxloc];    

    e_T = err( 1, maxloc ); 
    h_T = hs(:, maxloc);

    % Check
	if( abs(e_T) <= 0.005 )
		printf("Breaking in T: %d e_T: %f \n", j, e_T);
		break;
	endif

	alpha_T = (1/2)*log( (1-e_T)/e_T ); 

	% Update
    oldWeights = trainData(:,4);
    labels = trainData(:,3);
	newWeights = (oldWeights.*exp(-alpha_T*labels.*h_T) );
	Sw = sum( newWeights );
	trainData(:,4) = newWeights/Sw;	
    
endfor

weakClass

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Check output final classifier
printf("Checking accuracy \n");

ch = zeros(n,T);
H = zeros(n,1);
sch = zeros(n,1);

for i = 1:T
	ch(:,i) = alpha(i,1).*applyStump( trainData, weakClass(i,1), weakClass(i,2) );
    sch = sum(ch,2);
endfor

for i = 1:n
	if sch(i,1) < 0
		H(i,1) = -1;
	else
		H(i,1) = 1;
	endif
endfor

% Calculate errors
finalErrors = 0;

for i = 1:n
	if H(i,1) != trainData(i,3)
		finalErrors++;
	endif
endfor

printf("Misclassified %d out of %d \n with a T of: %d \n", finalErrors, n, T);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printf("Plotting");

% Plot train set
figure(10);
plot( trainA(:,1), trainA(:,2), 'mx', 'markersize', 5, 'linewidth', 5 );
hold on;
plot( trainB(:,1), trainB(:,2), 'bx', 'markersize', 5, 'linewidth', 5  );

for i = 1 : size(weakClass,1)

	if( weakClass(i,2) == 1 )
		plot( [weakClass(i,1), weakClass(i,1)], ylim, 'g', 'linewidth', 2  );
	elseif ( weakClass(i,2) == 2 )
		plot( [weakClass(i,1), weakClass(i,1)], ylim, 'c', 'linewidth', 2  );
    elseif ( weakClass(i,2) == 3 )
		plot( xlim, [weakClass(i,1), weakClass(i,1)], 'k', 'linewidth', 2  );
    elseif ( weakClass(i,2) == 4 )
		plot( xlim, [weakClass(i,1), weakClass(i,1)], 'r', 'linewidth', 2  );
	else
	    printf("Idiot! This should not happen \n");
	endif
    
endfor

	legend('Class A', 'Class B' );
	title('Train Set');
	print('proj6-3-1.png', '-dpng');
	hold off;


