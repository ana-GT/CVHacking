%% Octave

% Load train data
trainA = load("trainA_blank.txt");
trainB = load("trainB_blank.txt");

% Load test data
testA = load("testA_blank.txt");
testB = load("testB_blank.txt");

% Plot train set
figure(1);
plot( trainA(:,1), trainA(:,2), 'mx', 'markersize', 5, 'linewidth', 5 );
hold on;
plot( trainB(:,1), trainB(:,2), 'bx', 'markersize', 5, 'linewidth', 5  );
legend('Class A', 'Class B' );
title('Train Set');
print('proj6-1-1-TrainSet.png', '-dpng');
hold off;

% Plot test set
figure(2);
plot( testA(:,1), testA(:,2), 'rx', 'markersize', 5, 'linewidth', 5  );
hold on;
plot( testB(:,1), testB(:,2), 'gx', 'markersize', 5, 'linewidth', 5  );
legend('Class A', 'Class B' );
title('Test Set');
print('proj6-1-1-TestSet.png', '-dpng');
