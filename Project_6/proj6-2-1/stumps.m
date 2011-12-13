
printf("Loading stumps \n");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [xStumps, err, hs] = xStumps( data )

	n = size(data,1);

	% Find limits
	x1_min = min( data(:,1) );
	x1_max = max( data(:,1) );

	x2_min = min( data(:,2) );
	x2_max = max( data(:,2) );

	K = 100;
	dx1 = (x1_max - x1_min)/K;
	dx2 = (x2_max - x2_min)/K;
    dx = [dx1, dx1, dx2, dx2];

	% Find best slumps

	x_stumps = zeros(K,4);
	errors = zeros(K,4);

	x_stumps(1,1) = x1_min;
	x_stumps(1,2) = x1_min;
	x_stumps(1,3) = x2_min;
	x_stumps(1,4) = x2_min;


    % Do the job
	for i = 1:K

		predicted_x = zeros(n,4);

		% Classify
		for j = 1:n
			% stump 1
    		if data(j,1) < x_stumps(i,1)
		  		predicted_x(j,1) = 1; % A 
			else
		  		predicted_x(j,1) = -1; % B
			endif

			% stump 2
    		if data(j,1) < x_stumps(i,2)
		  		predicted_x(j,2) = -1; % B 
			else
		  		predicted_x(j,2) = 1; % A
			endif

			% stump 3
    		if data(j,2) < x_stumps(i,3)
		  		predicted_x(j,3) = 1; % A 
			else
		  		predicted_x(j,3) = -1; % B
			endif

			% stump 4
    		if data(j,2) < x_stumps(i,4)
		  		predicted_x(j,4) = -1; % B 
			else
		  		predicted_x(j,4) = 1; % A
			endif
		endfor 	

		% Check accuracy
    	counts = zeros(1,4);

		for j = 1:n

		    % Stump 1
    		if predicted_x(j,1) != data(j,3)
				counts(1,1) = counts(1,1) + data(j,4);
			endif

		    % Stump 2
    		if predicted_x(j,2) != data(j,3)
				counts(1,2) = counts(1,2) + data(j,4);
			endif

		    % Stump 3
    		if predicted_x(j,3) != data(j,3)
				counts(1,3) = counts(1,3) + data(j,4);
			endif

		    % Stump 4
    		if predicted_x(j,4) != data(j,3)
				counts(1,4) = counts(1,4) + data(j,4);
			endif

		endfor 	

		errors(i,1) = counts(1,1);
		errors(i,2) = counts(1,2);
		errors(i,3) = counts(1,3);
		errors(i,4) = counts(1,4);

		% Update stumps
		x_stumps(i+1,:) = x_stumps(i,:) .+ dx;
	endfor

	[minx1, minloc1] = min(errors(:,1) );
	[minx2, minloc2] = min(errors(:,2) );
	[minx3, minloc3] = min(errors(:,3) );
	[minx4, minloc4] = min(errors(:,4) );

    xStumps = zeros(1,4);
	xStumps(1,1) = x_stumps( minloc1, 1 );
	xStumps(1,2) = x_stumps( minloc2, 2 );
	xStumps(1,3) = x_stumps( minloc3, 3 );
	xStumps(1,4) = x_stumps( minloc4, 4 );

	err = zeros(1,4);
    err(1,1) = errors( minloc1, 1 );
    err(1,2) = errors( minloc2, 2 );
    err(1,3) = errors( minloc3, 3 );
    err(1,4) = errors( minloc4, 4 );

	% hs
	hs = zeros(n,4);

	for j = 1:n

   		if data(j,1) < xStumps(1,1)
	  		hs(j,1) = 1; % A 
		else
	  		hs(j,1) = -1; % B
		endif

   		if data(j,1) < xStumps(1,2)
	  		hs(j,2) = -1; % B 
		else
	  		hs(j,2) = 1; % A
		endif


   		if data(j,2) < xStumps(1,3)
	  		hs(j,3) = 1; % A 
		else
	  		hs(j,3) = -1; % B
		endif


   		if data(j,2) < xStumps(1,4)
	  		hs(j,4) = -1; % B 
		else
	  		hs(j,4) = 1; % A
		endif

	endfor 	

endfunction

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [h] = applyStump( data, stump, type )

    n = size(data,1);
    h = zeros(n,1);

    if type == 1

		for j = 1:n
   			if data(j,1) < stump
	  			h(j,1) = 1; % A 
			else
	  			h(j,1) = -1; % B
			endif
		endfor

	elseif type == 2

		for j = 1:n
	   		if data(j,1) < stump
		  		h(j,1) = -1; % B 
			else
		  		h(j,1) = 1; % A
			endif
		endfor

	elseif type == 3

		for j = 1:n
   			if data(j,2) < stump
	  			h(j,1) = 1; % A 
			else
	  			h(j,1) = -1; % B
			endif
		endfor

	elseif type == 4

		for j = 1:n
	   		if data(j,2) < stump
		  		h(j,1) = -1; % B 
			else
		  		h(j,1) = 1; % A
			endif
		endfor
	else
		printf("Silly, something is wrong, probably your input \n");
    endif


endfunction
