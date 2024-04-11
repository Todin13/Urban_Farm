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


print(10000*"_")

# Créer un objet curseur
cur = conn.cursor()


def inserer(valeurs):
    # Définir la requête SQL pour insérer une ligne dans la table
    query = "INSERT INTO SensorsData () VALUES (%s, %s, %s, %s, %s, %s, %s)"
    
    # Exécuter la requête SQL
    cur.execute(query, valeurs)
    print("Ligne insérée dans la table")

def terminer_insertions():
    # Valider la transaction
    conn.commit()

    # Fermer le curseur et la connexion
    cur.close()
    conn.close()


"""#!!! Test !!!
query = "SELECT * FROM SensorsData"
cur.execute(query)
rows = cur.fetchall()
print(10000*"_")
for row in rows:
    print(row)"""
