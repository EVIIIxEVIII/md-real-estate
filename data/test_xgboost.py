import pandas as pd
import numpy as np
from xgboost import XGBRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score

df = pd.read_csv("./datasets/encoded_df.csv")

X = df.drop(columns=['price'])
y = df['price']

X = pd.get_dummies(X)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.15, random_state=42)

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
	tree_method='hist',        # or 'gpu_hist' if you have GPU
	verbosity=1,
	n_jobs=-1,
	random_state=42
)

model.fit(X_train, y_train)

y_pred = model.predict(X_test)

mae = mean_absolute_error(y_test, y_pred)
mse = mean_squared_error(y_test, y_pred)
rmse = np.sqrt(mse)
r2 = r2_score(y_test, y_pred)

print("\nEvaluation Metrics:")
print(f"MAE  = {mae:.1f}")
print(f"MSE  = {mse:.5g}")
print(f"RMSE = {rmse:.1f}")
print(f"R^2  = {r2:.6f}")

