import pandas as pd

df_test = pd.read_csv("./test.csv")
df_pred = pd.read_csv("./predictions.csv")

print(df_test.head(11))
print(df_pred.head(11))

df_submission = pd.DataFrame({
    "Id": df_test["Id"],
    "SalePrice": df_pred["SalePrice"]
})

df_submission.to_csv("submission.csv", index=False)

