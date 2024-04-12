import streamlit as st
from time import sleep
import psycopg2
import os
import matplotlib.pyplot as plt
import pandas as pd


# Define the management of the app
def manage_session_state():
    if "page" not in st.session_state:
        st.session_state.page = "Home"

# Database connection
def connect_to_db():
    dbname = os.environ.get("POSTGRES_DB", "urbanfarm")
    user = os.environ.get("POSTGRES_USER", "admin")
    password = os.environ.get("POSTGRES_PASSWORD", "urbanfarm123")
    host = os.environ.get("POSTGRES_HOST", "localhost")
    port = os.environ.get("POSTGRES_PORT", "5433")

    try:
        conn = psycopg2.connect(
            dbname=dbname,
            user=user,
            password=password,
            host=host,
            port=port
        )
    except psycopg2.OperationalError as e:
        print(f"Error: Unable to connect to the database: {e}")
        conn = None

    return conn

# Function to display dashboard overview
# Function to display dashboard overview
def display_dashboard_overview(cursor):
    st.markdown("Dashboard Overview")

    # Fetch metrics from the database
    try:
        # Fetch average, minimum, and maximum temperature excluding null values
        cursor.execute("SELECT AVG(temperature), MIN(temperature), MAX(temperature) FROM SensorsData WHERE temperature IS NOT NULL;")
        avg_temp, min_temp, max_temp = cursor.fetchone()

        # Fetch average, minimum, and maximum humidity excluding null values
        cursor.execute("SELECT AVG(humidity), MIN(humidity), MAX(humidity) FROM SensorsData WHERE humidity IS NOT NULL;")
        avg_humidity, min_humidity, max_humidity = cursor.fetchone()

        # Fetch temperature and humidity data for plotting
        cursor.execute("SELECT Time, temperature, humidity FROM SensorsData WHERE temperature IS NOT NULL AND humidity IS NOT NULL ORDER BY Time;")
        data = cursor.fetchall()
        df = pd.DataFrame(data, columns=["Time", "Temperature", "Humidity"])
        df["Time"] = pd.to_datetime(df["Time"])

        # Count sensors online
        cursor.execute("SELECT COUNT(*) FROM Sensors;")
        total_sensors = cursor.fetchone()[0]
        cursor.execute("SELECT COUNT(*) FROM SensorsData WHERE Time > now() - interval '1 minute';")
        sensors_online = cursor.fetchone()[0]

        # Count active alarms
        cursor.execute("SELECT COUNT(*) FROM Warnings WHERE Type = 'Alarm';")
        active_alarms = cursor.fetchone()[0]

        cursor.close()
    except psycopg2.Error as e:
        print(f"Error fetching data from the database: {e}")
        st.error("An error occurred while fetching data from the database.")
        return

    # Display metrics
    st.subheader("Metrics:")
    if avg_temp is not None:
        st.write(f"- Average Temperature: {avg_temp:.2f}°C")
    if min_temp is not None:
        st.write(f"- Minimum Temperature: {min_temp}°C")
    if max_temp is not None:
        st.write(f"- Maximum Temperature: {max_temp}°C")
    if avg_humidity is not None:
        st.write(f"- Average Humidity: {avg_humidity:.2f}%")
    if min_humidity is not None:
        st.write(f"- Minimum Humidity: {min_humidity}%")
    if max_humidity is not None:
        st.write(f"- Maximum Humidity: {max_humidity}%")


    # Plot temperature and humidity evolution
    st.subheader("Temperature and Humidity Evolution")
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(df["Time"], df["Temperature"], label="Temperature", color="blue")
    ax.plot(df["Time"], df["Humidity"], label="Humidity", color="green")
    ax.set_xlabel("Time")
    ax.set_ylabel("Value")
    ax.set_title("Temperature and Humidity Evolution")
    ax.legend()
    st.pyplot(fig)

# Function to fetch sensor data
def fetch_sensor_data(cursor, sensor_id):
    try:
        cursor.execute(f"SELECT * FROM SensorsData WHERE Sensor_ID = {sensor_id};")
        data = cursor.fetchall()
        return data
    except psycopg2.Error as e:
        st.error(f"Error fetching sensor data: {e}")
        return None

# Function to display sensor section
def display_sensor_section(cursor):
    st.markdown("Sensors Section")

    # Fetch sensor data
    try:
        cursor.execute("SELECT ID, Version FROM sensors;")
        sensor_data = cursor.fetchall()
    except psycopg2.Error as e:
        st.error(f"Error fetching sensor data: {e}")
        return

    # Display sensor table
    st.write("Sensor Table:")
    sensor_df = pd.DataFrame(sensor_data, columns=["Sensor ID", "Version"])
    st.dataframe(sensor_df)

    # Handle sensor selection
    selected_sensor_id = st.selectbox("Select a sensor ID to view its data:", sensor_df["Sensor ID"])
    if st.button("View Sensor Data"):
        sensor_data = fetch_sensor_data(cursor, selected_sensor_id)
        if sensor_data:
            st.write("Sensor Data:")
            sensor_data_df = pd.DataFrame(sensor_data, columns=["ID", "Sensor_ID", "Sensor_Version", "PlantID", "Time", "Temperature", "Humidity"])
            st.dataframe(sensor_data_df)

# Function to display data analysis section
def display_data_analysis_section(cursor):
    st.markdown("Data Analysis Section")
    # Display data analysis options and graphs

# Function to display alarms section
def display_alarms_section(cursor):
    st.markdown("Alarms Section")
    # Fetch and display active alarms from the database

# Main function
def main():

    manage_session_state()

    st.set_option('deprecation.showPyplotGlobalUse', False)

    st.title("Urban Farm Monitoring")

    # Render navigation menu
    st.sidebar.title("Navigation Menu")
    pages = ["Home", "Sensors", "Data Analysis", "Alarms"]
    selected_page = st.sidebar.radio("Go to", pages, index=pages.index(st.session_state.page))

    # Rerun the app only when the selected page is different
    if selected_page != st.session_state.page:
        st.session_state.page = selected_page
        st.rerun()


    # Connect to the database
    conn = connect_to_db()
    while not conn :
        conn = connect_to_db()
        st.write("Connecting to the database ...")
        sleep(2)
        st.rerun()
    cursor = conn.cursor()

    # Display content based on navigation choice
    if st.session_state.page == "Home":
        display_dashboard_overview(cursor)
    elif st.session_state.page == "Sensors":
        display_sensor_section(cursor)
    elif st.session_state.page == "Data Analysis":
        display_data_analysis_section(cursor)
    elif st.session_state.page == "Alarms":
        display_alarms_section(cursor)

    # Close database connection
    cursor.close()
    conn.close()

if __name__ == "__main__":
    main()
