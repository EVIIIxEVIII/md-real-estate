# C++ application for real estate price analysis in Moldova
Data was collected from 999.md. The model used is a Gradient Boosted Trees algorithm.

![image](https://github.com/user-attachments/assets/bf3b2be8-9a2f-4f5b-8fcb-d30ca4b0bb39)


# Comparison with existing models:
- my extreme gradient boosted forest implementation:
```
max depth = 6
estimators = 350
eta = 0.3
lambda = 5.0
gamma = 0.0
```
Result:
MAE   = 17148.7 | 
MSE   = 9.346e+08 | 
RMSE  = 30571.2 | 
R^2   = 0.80469 | 
RMSLE = 0.215884 

****
- xgboost python library:

Parameters
```python
model = XGBRegressor(
    n_estimators=1000,
	learning_rate=0.01,
	max_depth=6,
	min_child_weight=1,
	subsample=0.8,
	colsample_bytree=0.8,
	gamma=0,
	reg_alpha=0.1,
	reg_lambda=1.0,
	objective='reg:squarederror',
	tree_method='hist',
	verbosity=1,
	n_jobs=-1,
	random_state=42
)
```
Result:
MAE  = 18184.8 |
MSE  = 1.0122e+09 | 
RMSE = 31814.8 | 
R^2  = 0.814366

****
- my ols implementation:

Result:
MAE  = 24259.4 | 
MSE  = 1.62136e+09 | 
RMSE = 40266.1 | 
R^2  = 0.688085 | 
