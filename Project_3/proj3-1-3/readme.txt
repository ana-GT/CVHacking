Observations:

1. With K = 8, the residuals are generally higher than for K = 12 or K = 15. This is somewhat expected since we are fitting fewer points, hence the expression is less accurate.
2. With K = 12 the residuals gets smaller, same case for K = 15. However, once in a while an outlier appears( check Trial [17] with K = 12 with a big residual compared to others)
3. I found that running the trials multiple times vary the matrix M, but only slightly. The min residual varies accordingly (based on my observation between 0.45 - 1.2 approx). 


Trial [0] -- K: 8 Residual: 1.764431 
Trial [1] -- K: 8 Residual: 4.055505 
Trial [2] -- K: 8 Residual: 3.216198 
Trial [3] -- K: 8 Residual: 1.820099 
Trial [4] -- K: 8 Residual: 1.483361 
Trial [5] -- K: 8 Residual: 1.259302 
Trial [6] -- K: 8 Residual: 2.996320 
Trial [7] -- K: 8 Residual: 0.959180 
Trial [8] -- K: 8 Residual: 1.464612 
Trial [9] -- K: 8 Residual: 1.298071 
Trial [10] -- K: 12 Residual: 1.109463 
Trial [11] -- K: 12 Residual: 1.195833 
Trial [12] -- K: 12 Residual: 0.710614 
Trial [13] -- K: 12 Residual: 0.860647 
Trial [14] -- K: 12 Residual: 1.034618 
Trial [15] -- K: 12 Residual: 1.375425 
Trial [16] -- K: 12 Residual: 0.544843 
Trial [17] -- K: 12 Residual: 5.193568 
Trial [18] -- K: 12 Residual: 1.256854 
Trial [19] -- K: 12 Residual: 0.998846 
Trial [20] -- K: 15 Residual: 1.337176 
Trial [21] -- K: 15 Residual: 1.110312 
Trial [22] -- K: 15 Residual: 1.194604 
Trial [23] -- K: 15 Residual: 1.153852 
Trial [24] -- K: 15 Residual: 1.397920 
Trial [25] -- K: 15 Residual: 1.072206 
Trial [26] -- K: 15 Residual: 1.195618 
Trial [27] -- K: 15 Residual: 0.713192 
Trial [28] -- K: 15 Residual: 1.325023 
Trial [29] -- K: 15 Residual: 1.589503 

 Min residual found with k: 12
 Min residual value: 0.544843
 Matrix M: 
-0.178791  0.503007 0.0121929 0.0134698
0.0871845  0.020935  0.527886 0.0273949
-0.271446 -0.132274 0.0673814 -0.575773

 Matrix T2: 
0.00223564          0   -1.37872
         0 0.00223564  -0.775542
         0          0          1
 Matrix T3: 
0.283844        0        0 -87.7714
       0 0.283844        0 -87.8424
       0        0 0.283844 -8.37431
       0        0        0        1

