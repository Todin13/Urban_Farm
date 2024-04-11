import psycopg2
from faker import Faker
import random

# Database connection parameters
conn_params = {
    "dbname": "urbanfarm",
    "user": "admin",
    "password": "urbanfarm123",
    "host": "localhost",
    "port": 5433
}

def populate_db():
    """Populate the database with generated test data."""
    fake = Faker()
    conn = None
    try:
        print("Connecting to the PostgreSQL database...")
        conn = psycopg2.connect(**conn_params)

        # Create a cursor for performing database operations
        cur = conn.cursor()
        print("Connection successful")

        # Generate data for the Experiment table
        #for i in range(5):  # Example: 5 experiment entries
        #    experiment_id = i + 1
        #    experiment_name = fake.word() + " Experiment"
        #    cur.execute("""
        #        INSERT INTO Experiment (ID, Name) VALUES (%s, %s)
        #        ON CONFLICT (ID) DO UPDATE SET
        #        Name = EXCLUDED.Name;
        #    """, (experiment_id, experiment_name))

        # Generate data for the Sensors table
        for i in range(10):  # Example: 10 sensor entries
            sensor_id = i + 1
            sensor_version = fake.random_element(elements=('FR-v8', 'EN-v2'))

            cur.execute("""
                INSERT INTO Sensors (ID, Version) VALUES (%s, %s)
                ON CONFLICT (ID) DO UPDATE SET
                Version = EXCLUDED.Version;
            """, (sensor_id, sensor_version))

        # Generate data for the SensorsData table
        for i in range(20):  # Example: 20 data entries
            data_id = i + 1
            sensor_id = random.randint(1, 10)  # Assuming 10 sensors
            plant_id = random.randint(1, 5)    # Assuming 5 different plants
            temperature = round(random.uniform(15.0, 35.0), 2)
            humidity = round(random.uniform(30.0, 80.0), 2)
            time_of_record = fake.date_time_this_year()
            sensor_version = fake.random_element(elements=('FR-v8', 'EN-v2'))

            cur.execute("""
                INSERT INTO SensorsData (ID, Sensor_ID, Sensor_Version,Plant_ID, Time, Temperature, Humidity)
                VALUES (%s, %s, %s, %s, %s, %s, %s)
                ON CONFLICT (ID) DO UPDATE SET
                Temperature = EXCLUDED.Temperature,
                Humidity = EXCLUDED.Humidity,
                Time = EXCLUDED.Time;
            """, (data_id, sensor_id, sensor_version, plant_id, time_of_record, temperature, humidity))

        # Commit changes
        conn.commit()

        # Close communication with the database
        cur.close()
    except (Exception, psycopg2.DatabaseError) as error:
        print('Error:', error)
    finally:
        if conn is not None and conn.closed == 0:
            conn.close()
            print("Database connection closed.")

if __name__ == '__main__':
    populate_db()
