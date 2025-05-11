import pandas as pd
from xgboost import XGBRegressor

# ---------- paths ----------
train_path = "./kaggle-test/train_filtered.csv"
test_path  = "./kaggle-test/test_filtered.csv"

# ---------- load ----------
df_train = pd.read_csv(train_path)
df_test  = pd.read_csv(test_path)

# ---------- separate target ----------
y_train = df_train["price"]
X_train = df_train.drop(columns=["price"])

# ---------- train ----------
model = XGBRegressor(
    n_estimators     = 1000,
    learning_rate    = 0.01,
    max_depth        = 6,
    subsample        = 0.8,
    colsample_bytree = 0.8,
    objective        = "reg:squarederror",
    tree_method      = "hist",
    n_jobs           = -1,
    random_state     = 42
)
model.fit(X_train, y_train)

# ---------- predict test ----------
# keep Id column for submission, drop it from features
test_ids = df_test["Id"]                      # must exist per your note
X_test   = df_test.drop(columns=["Id"])

# make sure train & test have identical columns/order
X_test = X_test.reindex(columns=X_train.columns, fill_value=0)

preds = model.predict(X_test)

# ---------- save ----------
submission = pd.DataFrame({"Id": test_ids, "price": preds})
submission.to_csv("predictions.csv", index=False)
print(f"Saved predictions.csv ({len(preds)} rows)")

