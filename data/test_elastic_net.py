import pandas as pd
import numpy as np
from sklearn.linear_model import ElasticNet
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score

# Load your filtered dataset
df = pd.read_csv("./datasets/encoded_df.csv")

# Separate features and target
y = df["price"].values
X = df.drop(columns=["price"]).copy()

# Add intercept manually as first column
X.insert(0, 'intercept', 1.0)

# Convert to NumPy only after all manipulations
X = X.values

# Deterministic 75/25 split like your C++ code
split_index = int(len(X) * 0.75)
X_train, X_test = X[:split_index], X[split_index:]
y_train, y_test = y[:split_index], y[split_index:]

# Match C++ λ₁ = 2.0 and λ₂ = 3.0
alpha = 5.0
l1_ratio = 2.0 / 5.0

# Train ElasticNet model
model = ElasticNet(
	alpha=alpha,
	l1_ratio=l1_ratio,
	max_iter=1000,
	tol=1e-6,
	random_state=42,
	fit_intercept=False  # you're manually including intercept in X
)

model.fit(X_train, y_train)
y_pred = model.predict(X_test)

# Evaluate
mae = mean_absolute_error(y_test, y_pred)
mse = mean_squared_error(y_test, y_pred)
rmse = np.sqrt(mse)
r2 = r2_score(y_test, y_pred)

print("\nEvaluation Metrics:")
print(f"MAE  = {mae:.1f}")
print(f"MSE  = {mse:.5g}")
print(f"RMSE = {rmse:.1f}")
print(f"R^2  = {r2:.6f}")

