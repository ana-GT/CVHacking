/** --------------- */
/** Question 3-1-3  */
/** --------------- */

Observations:
-------------

1. With K = 8, the residuals are generally higher than for K = 12 or K = 15. This is somewhat expected since we are fitting fewer points, hence the expression is less accurate. Check out my first trial, the residual is huge! (well, nearly 5 pixels..)
2. With K = 12 the residuals gets smaller, same case for K = 15. However, once in a while an outlier result appear. In this particular running the values are pretty much consistent.
3. I found that running the trials multiple times vary the matrix M, but only slightly. This depending of the group of random points considered. The residual for the 5 point tests left  varies accordingly (based on my observation between 0.44 - 1.5 approx). 

Trial [0] -- K: 8 Residual: 4.831602 
Trial [1] -- K: 8 Residual: 1.154022 
Trial [2] -- K: 8 Residual: 1.675405 
Trial [3] -- K: 8 Residual: 1.279778 
Trial [4] -- K: 8 Residual: 1.949832 
Trial [5] -- K: 8 Residual: 10.853393 
Trial [6] -- K: 8 Residual: 2.262367 
Trial [7] -- K: 8 Residual: 1.652599 
Trial [8] -- K: 8 Residual: 1.093229 
Trial [9] -- K: 8 Residual: 0.654903 
Trial [10] -- K: 12 Residual: 0.990855 
Trial [11] -- K: 12 Residual: 1.136755 
Trial [12] -- K: 12 Residual: 1.174034 
Trial [13] -- K: 12 Residual: 2.242073 
Trial [14] -- K: 12 Residual: 1.677446 
Trial [15] -- K: 12 Residual: 2.264712 
Trial [16] -- K: 12 Residual: 1.265488 
Trial [17] -- K: 12 Residual: 1.808802 
Trial [18] -- K: 12 Residual: 1.113692 
Trial [19] -- K: 12 Residual: 3.729320 
Trial [20] -- K: 15 Residual: 1.052136 
Trial [21] -- K: 15 Residual: 0.928273 
Trial [22] -- K: 15 Residual: 1.147938 
Trial [23] -- K: 15 Residual: 1.374408 
Trial [24] -- K: 15 Residual: 1.156444 
Trial [25] -- K: 15 Residual: 1.452683 
Trial [26] -- K: 15 Residual: 1.233854 
Trial [27] -- K: 15 Residual: 0.923310 
Trial [28] -- K: 15 Residual: 1.244299 
Trial [29] -- K: 15 Residual: 0.448924
 
*  Min residual found with k: 15
*  Min residual value: 0.448924

*  Matrix M: 

-0.178488  0.503105 0.0121251  0.013187
0.0871701 0.0208584  0.527866  0.027384
-0.271481 -0.132064  0.067756 -0.575801

* Matrix T2: 
0.00223564          0   -1.37872
         0 0.00223564  -0.775542
         0          0          1
* Matrix T3: 
0.283844        0        0 -87.7714
       0 0.283844        0 -87.8424
       0        0 0.283844 -8.37431
       0        0        0        1


* Residual of all the points using our new found M:

[0] P2D: (731 , 238) -- Predicted: (731.3160 , 238.3278) - Residual: (0.3160 0.3278) Mod: 0.4553 
[1] P2D: (22 , 248) -- Predicted: (22.6823 , 248.1022) - Residual: (0.6823 0.1022) Mod: 0.6899 
[2] P2D: (204 , 230) -- Predicted: (204.2721 , 229.7202) - Residual: (0.2721 -0.2798) Mod: 0.3903 
[3] P2D: (903 , 342) -- Predicted: (902.4904 , 342.1687) - Residual: (-0.5096 0.1687) Mod: 0.5368 
[4] P2D: (635 , 316) -- Predicted: (636.0295 , 316.5600) - Residual: (1.0295 0.5600) Mod: 1.1720 
[5] P2D: (867 , 177) -- Predicted: (866.7927 , 176.4527) - Residual: (-0.2073 -0.5473) Mod: 0.5853 
[6] P2D: (958 , 572) -- Predicted: (958.7755 , 571.9453) - Residual: (0.7755 -0.0547) Mod: 0.7775 
[7] P2D: (328 , 244) -- Predicted: (327.0649 , 244.3034) - Residual: (-0.9351 0.3034) Mod: 0.9831 
[8] P2D: (426 , 386) -- Predicted: (425.2542 , 385.8181) - Residual: (-0.7458 -0.1819) Mod: 0.7676 
[9] P2D: (1064 , 470) -- Predicted: (1062.9790 , 470.2710) - Residual: (-1.0210 0.2710) Mod: 1.0564 
[10] P2D: (480 , 495) -- Predicted: (480.1122 , 495.3786) - Residual: (0.1122 0.3786) Mod: 0.3948 
[11] P2D: (964 , 419) -- Predicted: (965.2688 , 419.5101) - Residual: (1.2688 0.5101) Mod: 1.3675 
[12] P2D: (695 , 374) -- Predicted: (693.7239 , 374.2441) - Residual: (-1.2761 0.2441) Mod: 1.2993 
[13] P2D: (505 , 372) -- Predicted: (505.5673 , 371.7841) - Residual: (0.5673 -0.2159) Mod: 0.6070 
[14] P2D: (645 , 452) -- Predicted: (644.7752 , 452.4254) - Residual: (-0.2248 0.4254) Mod: 0.4811 
[15] P2D: (692 , 359) -- Predicted: (690.9385 , 358.0084) - Residual: (-1.0615 -0.9916) Mod: 1.4526 
[16] P2D: (712 , 444) -- Predicted: (713.4903 , 443.8631) - Residual: (1.4903 -0.1369) Mod: 1.4966 
[17] P2D: (465 , 263) -- Predicted: (465.2816 , 263.2855) - Residual: (0.2816 0.2855) Mod: 0.4010 
[18] P2D: (591 , 324) -- Predicted: (591.5412 , 323.8252) - Residual: (0.5412 -0.1748) Mod: 0.5687 
[19] P2D: (447 , 213) -- Predicted: (446.6584 , 213.1157) - Residual: (-0.3416 0.1157) Mod: 0.3607 


