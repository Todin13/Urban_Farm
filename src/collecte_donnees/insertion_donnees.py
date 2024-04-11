import psycopg2
from time import sleep


# Établir une connexion à la base de données
conn = None
while conn is None:
    try:
        conn = psycopg2.connect(
            host="database",
            port=5432,
            dbname="urbanfarm",
            user="admin",
            password="urbanfarm123"
        )
    except psycopg2.OperationalError:
        sleep(1)


print(1000*"_")


def inserer(donnees):
    with conn.cursor() as cur:
        # Définir la requête SQL pour insérer une ligne dans la table        
        query = f"INSERT INTO SensorsData ({', '.join(donnees.keys())}) VALUES ({', '.join(map(repr, donnees.values()))})"
    
        print(query)
    
        # Exécuter la requête SQL
        cur.execute(query)

        # Valider la transaction
        conn.commit()

        print("Ligne insérée dans la table")


"""#!!! Test !!!
query = "SELECT * FROM SensorsData"
cur.execute(query)
rows = cur.fetchall()
print(10000*"_")
for row in rows:
    print(row)"""
