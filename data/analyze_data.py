import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./raw_data.csv")

# Animale,Autorul anunțului,Balcon/ lojie,Compartimentare,Copii,Dezvoltator,Etaj,Fond locativ,Grup sanitar,Inălțimea tavanelor,Living,Loc de parcare,Număr de camere,Număr de etaje,Starea apartamentului,Suprafață bucătărie,Suprafață locativă,Suprafață totală,Tip clădire,price
def normalize_units(x):
	if isinstance(x, str):
		parts = x.strip().split()
		if len(parts) == 2:
			value, unit = parts
			try:
				value = float(value)
				if unit.lower() in ['cm', 'centimetri']:
					return value / 100
				elif unit.lower() in ['m', 'metri']:
					return value
			except ValueError:
				return None
	return None

number_of_rooms_types = {
    "O cameră": 1,
    "Apartament cu 1 cameră": 2,
    "Apartament cu 2 camere": 3,
    "Apartament cu 3 camere": 4,
    "Apartament cu 4 camere": 5,
    "Apartament cu 5 camere sau mai multe": 6,
}

fields_map = {
	"Animale": "animals",
	"Autorul anunțului": "author_of_the_announcement",
	"Balcon/ lojie": "balcony_loggia",
	"Compartimentare": "compartmentalization",
	"Copii": "children",
	"Dezvoltator": "developer",
	"Etaj": "floor",
	"Fond locativ": "housing_fund",
	"Grup sanitar": "bathroom",
	"Inălțimea tavanelor": "ceiling_height",
	"Living": "living_room",
	"Loc de parcare": "parking_spot",
	"Număr de camere": "number_of_rooms",
	"Număr de etaje": "number_of_floors",
	"Starea apartamentului": "apartment_condition",
	"Suprafață bucătărie": "kitchen_area",
	"Suprafață locativă": "living_area",
	"Suprafață totală": "total_area",
	"Tip clădire": "building_type",
	"price": "price"
}

df = df.drop_duplicates()
df.to_csv("./datasets/no_duplicate_data.csv", index=False)

df = df.rename(columns=fields_map)

df = df[df["price"].str.strip().str.split().str[-1].str[-1] == "€"]
df['price'] = df["price"].str[:-1]
df['price'] = df["price"].apply(lambda x: int("".join(x.strip().split())))
df = df[df['price'] > 15_000]

df['total_area'] = df['total_area'].apply(lambda x: int("".join(x.strip().split()[0])))
df['ceiling_height'] = df['ceiling_height'].apply(normalize_units)
df['ceiling_height'] = pd.to_numeric(df['ceiling_height'], errors='coerce')
df['number_of_rooms'] = df['number_of_rooms'].apply(lambda x: number_of_rooms_types[x] if x is not None else "")
df['number_of_rooms'] = pd.to_numeric(df['number_of_rooms'], errors='coerce')

df['is_penthouse'] = (df['floor'].str.lower() == 'penthouse').astype(int)
df['floor'] = pd.to_numeric(df['floor'], errors='coerce')

median_floor = df['floor'].median()
df['floor'] = df['floor'].fillna(median_floor)

df['compartmentalization'] = df['compartmentalization'].fillna("Individuală")

df['bathroom'] = pd.to_numeric(df['bathroom'], errors='coerce')
df['bathroom'] = df['bathroom'].fillna(1)

df = df[df['total_area'] < 500]
df = df[df['price'] < 750_000]

df.to_csv("./datasets/final_data.csv", index=False)

df_main_fields = df[[
    'price',
    'total_area',
    'number_of_rooms',
    'apartment_condition',
    'housing_fund',
    'region',
    'living_room',
    'parking_spot',
    'number_of_floors',
    'bathroom',
    'floor',
    'is_penthouse',
    'compartmentalization',
]]

df_encoded = pd.get_dummies(
    df_main_fields,
    columns=['apartment_condition', 'housing_fund', 'region', 'living_room', 'parking_spot', 'compartmentalization'],
    drop_first=True
).astype(int)

df_encoded.to_csv('./datasets/encoded_df.csv', index=False);

columns = [col for col in df_encoded.columns if col != "price"]
string = ""
for i, col in enumerate(columns):
    end = "," if i < len(columns) - 1 else ""
    string += f'"{col}"{end}';

print(string)

print(df[df['is_penthouse'] == 1])
