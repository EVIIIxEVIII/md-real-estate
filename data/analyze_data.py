import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./renamed_fields.csv")

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

df = df[df["price"].str.strip().str.split().str[-1].str[-1] == "€"]
df['price'] = df["price"].str[:-1]
df['price'] = df["price"].apply(lambda x: int("".join(x.strip().split())))
df = df[df['price'] > 10_000]


df['total_area'] = df['total_area'].apply(lambda x: int("".join(x.strip().split()[0])))
df['ceiling_height'] = df['ceiling_height'].apply(normalize_units)
df['ceiling_height'] = pd.to_numeric(df['ceiling_height'], errors='coerce')
df['number_of_rooms'] = df['number_of_rooms'].apply(lambda x: number_of_rooms_types[x] if x is not None else "")
df['number_of_rooms'] = pd.to_numeric(df['number_of_rooms'], errors='coerce')

df = df[df['total_area'] < 500]
df = df[df['price'] < 1_000_000]

df.to_csv("./datasets/final_data.csv", index=False)

plt.scatter(df['total_area'], df['price'])
plt.show()

plt.scatter(df['ceiling_height'], df['price'])
plt.show()

plt.scatter(df['number_of_rooms'], df['price'])
plt.show()



