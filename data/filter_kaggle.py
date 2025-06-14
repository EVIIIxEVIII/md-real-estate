import pandas as pd

train = pd.read_csv("./kaggle-test/train.csv")
test = pd.read_csv("./kaggle-test/test.csv")

for col in train.select_dtypes(include='object').columns:
	train[col] = train[col].astype('category').cat.codes
train.to_csv('./kaggle-test/train_filtered.csv', index=False);

for col in test.select_dtypes(include='object').columns:
	test[col] = test[col].astype('category').cat.codes
test.to_csv('./kaggle-test/test_filtered.csv', index=False);

columns = [col for col in train.columns if col != "price"]
string = ""
for i, col in enumerate(columns):
    end = "," if i < len(columns) - 1 else ""
    string += f'"{col}"{end}';

print(string)
