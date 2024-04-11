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

#with conn.cursor() as cur:
#    cur.execute("TRUNCATE SensorsData;")

with conn.cursor() as cur:
    cur.execute("SELECT MAX(ID) FROM SensorsData")
    sensorsdata_id = cur.fetchone()[0]
if sensorsdata_id is None:
    sensorsdata_id = 1
else:
    sensorsdata_id+=1

def inserer(donnees):
    global sensorsdata_id
    sensor_id = donnees["Sensor_ID"] = int(donnees["Sensor_ID"])
    donnees["ID"] = sensorsdata_id
    sensorsdata_id+=1
    with conn.cursor() as cur:
        cur.execute(f"INSERT INTO Sensors VALUES {sensor_id, donnees['Sensor_Version']} ON CONFLICT DO NOTHING")
        
        # Définir la requête SQL pour insérer une ligne dans la table        
        query = f"INSERT INTO SensorsData ({', '.join(donnees.keys())}) VALUES ({', '.join(map(repr, donnees.values()))})"
        
        # Exécuter la requête SQL
        cur.execute(query)

        # Valider la transaction
        conn.commit()

        print("Ligne insérée dans la table")
        
        """query = "SELECT * FROM SensorsData"
        cur.execute(query)
        rows = cur.fetchall()
        for row in rows:
            print(row)"""
