import os
import psycopg2
from time import sleep

# Get database connection parameters from environment variables
db_host = os.getenv("DB_HOST")
db_port = os.getenv("DB_PORT")
db_name = os.getenv("DB_NAME")
db_user = os.getenv("DB_USER")
db_password = os.getenv("DB_PASSWORD")

# Établir une connexion à la base de données
conn = None
while conn is None:
    try:
        conn = psycopg2.connect(
            host=db_host,
            port=db_port,
            dbname=db_name,
            user=db_user,
            password=db_password
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

        print(f"Ligne insérée dans la table : {donnees}")
        
        """query = "SELECT * FROM SensorsData"
        cur.execute(query)
        rows = cur.fetchall()
        for row in rows:
            print(row)"""
