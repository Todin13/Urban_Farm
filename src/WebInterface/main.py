import os
from time import sleep

import matplotlib.pyplot as plt
import pandas as pd
import psycopg2
import seaborn as sns  # type: ignore
import streamlit as st


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
            dbname=dbname, user=user, password=password, host=host, port=port
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
        cursor.execute(
            "SELECT AVG(temperature), MIN(temperature), MAX(temperature) FROM SensorsData WHERE temperature IS NOT NULL;"
        )
        avg_temp, min_temp, max_temp = cursor.fetchone()

        # Fetch average, minimum, and maximum humidity excluding null values
        cursor.execute(
            "SELECT AVG(humidity), MIN(humidity), MAX(humidity) FROM SensorsData WHERE humidity IS NOT NULL;"
        )
        avg_humidity, min_humidity, max_humidity = cursor.fetchone()

        # Fetch temperature and humidity data for plotting
        cursor.execute(
            "SELECT Time, temperature, humidity FROM SensorsData WHERE temperature IS NOT NULL AND humidity IS NOT NULL ORDER BY Time;"
        )
        data = cursor.fetchall()
        df = pd.DataFrame(data, columns=["Time", "Temperature", "Humidity"])
        df["Time"] = pd.to_datetime(df["Time"])

        # Count sensors online
        cursor.execute("SELECT COUNT(*) FROM Sensors;")
        total_sensors = cursor.fetchone()[0]
        cursor.execute(
            "SELECT COUNT(DISTINCT Sensor_ID) FROM SensorsData WHERE Time > now() - interval '1 min';"
        )
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
    st.write(f"Number of sensor online: {sensors_online} / {total_sensors}")
    st.write(f"Number of alarms: {active_alarms}")
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

    # Plot temperature evolution
    # Calculate rolling mean, max, and min values for temperature and humidity
    rolling_mean_temp = df["Temperature"].rolling(window=60).mean()
    rolling_max_temp = df["Temperature"].rolling(window=60).max()
    rolling_min_temp = df["Temperature"].rolling(window=60).min()

    rolling_mean_humidity = df["Humidity"].rolling(window=60).mean()
    rolling_max_humidity = df["Humidity"].rolling(window=60).max()
    rolling_min_humidity = df["Humidity"].rolling(window=60).min()

    # Plot temperature evolution
    st.subheader("Temperature Evolution")
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(
        df["Time"],
        rolling_mean_temp,
        label="Mean Temperature",
        linestyle="--",
        color="orange",
    )
    ax.plot(
        df["Time"],
        rolling_max_temp,
        label="Max Temperature",
        linestyle="-.",
        color="red",
    )
    ax.plot(
        df["Time"],
        rolling_min_temp,
        label="Min Temperature",
        linestyle=":",
        color="green",
    )
    ax.set_xlabel("Time")
    ax.set_ylabel("Temperature")
    ax.set_title("Temperature Evolution")
    ax.legend()
    st.pyplot(fig)

    # Plot humidity evolution
    st.subheader("Humidity Evolution")
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(
        df["Time"],
        rolling_mean_humidity,
        label="Mean Humidity",
        linestyle="--",
        color="orange",
    )
    ax.plot(
        df["Time"],
        rolling_max_humidity,
        label="Max Humidity",
        linestyle="-.",
        color="red",
    )
    ax.plot(
        df["Time"],
        rolling_min_humidity,
        label="Min Humidity",
        linestyle=":",
        color="green",
    )
    ax.set_xlabel("Time")
    ax.set_ylabel("Humidity")
    ax.set_title("Humidity Evolution")
    ax.legend()
    st.pyplot(fig)


# Function to fetch sensor data
def fetch_sensor_data(cursor, sensor_id):
    try:
        cursor.execute(f"SELECT * FROM SensorsData WHERE Sensor_ID = {sensor_id};")
        data = cursor.fetchall()
        return data
    except psycopg2.Error as e:
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
    selected_sensor_id = st.selectbox(
        "Select a sensor ID to view its data:", sensor_df["Sensor ID"]
    )
    if st.button("View Sensor Data"):
        sensor_data = fetch_sensor_data(cursor, selected_sensor_id)
        if sensor_data:
            st.write("Sensor Data:")
            sensor_data_df = pd.DataFrame(
                sensor_data,
                columns=[
                    "ID",
                    "Sensor_ID",
                    "Sensor_Version",
                    "PlantID",
                    "Time",
                    "Temperature",
                    "Humidity",
                    "Processed"
                ],
            )
            st.dataframe(sensor_data_df)

            # Plot temperature and humidity trends
            st.subheader("Temperature and Humidity Trends")
            fig, ax = plt.subplots(figsize=(10, 6))
            ax.plot(
                sensor_data_df["Time"],
                sensor_data_df["Temperature"],
                label="Temperature",
                color="blue",
            )
            ax.plot(
                sensor_data_df["Time"],
                sensor_data_df["Humidity"],
                label="Humidity",
                color="green",
            )
            ax.set_xlabel("Time")
            ax.set_ylabel("Value")
            ax.set_title("Temperature and Humidity Trends")
            ax.legend()
            st.pyplot(fig)


# Function to fetch data for data analysis
def fetch_data_for_analysis(cursor, time_range, granularity, selected_sensors):
    try:
        selected_sensors_condition = ""
        if selected_sensors:
            selected_sensors_condition = " AND sensor_id NOT IN %s"

        if time_range == "Last 24 Hours":
            cursor.execute(
                "SELECT Time, Temperature, Humidity, sensor_id FROM SensorsData WHERE Time > now() - interval '24 hours'"
                + selected_sensors_condition
                + ";",
                (tuple(selected_sensors),),
            )
        elif time_range == "Last 7 Days":
            cursor.execute(
                "SELECT Time, Temperature, Humidity, sensor_id FROM SensorsData WHERE Time > now() - interval '7 days'"
                + selected_sensors_condition
                + ";",
                (tuple(selected_sensors),),
            )
        elif time_range == "Last 30 Days":
            cursor.execute(
                "SELECT Time, Temperature, Humidity, sensor_id FROM SensorsData WHERE Time > now() - interval '30 days'"
                + selected_sensors_condition
                + ";",
                (tuple(selected_sensors),),
            )
        elif time_range == "All Time" and selected_sensors:
            cursor.execute(
                "SELECT Time, Temperature, Humidity, sensor_id FROM SensorsData WHERE sensor_id NOT IN %s ;",
                (tuple(selected_sensors),),
            )
        elif time_range == "All Time":
            cursor.execute(
                "SELECT Time, Temperature, Humidity, sensor_id FROM SensorsData;",
                (tuple(selected_sensors),),
            )

        data = cursor.fetchall()

        df = pd.DataFrame(
            data, columns=["Time", "Temperature", "Humidity", "SensorName"]
        )
        df["Time"] = pd.to_datetime(df["Time"])
        if granularity == "Hourly":
            df.set_index("Time", inplace=True)
            df = df.resample("1h").mean()
            df = df.reset_index()
        elif granularity == "Daily":
            df.set_index("Time", inplace=True)
            df = df.resample("1D").mean()
            df = df.reset_index()
        elif granularity == "Minute":
            df.set_index("Time", inplace=True)
            df = df.resample("1T").mean()  # 1T stands for 1 minute
            df = df.reset_index()
        elif granularity == "5 Minutes":
            df.set_index("Time", inplace=True)
            df = df.resample("5T").mean()  # 5T stands for 5 minutes
            df = df.reset_index()
        return df
    except psycopg2.Error as e:
        st.error(f"Error fetching data for analysis: {e}")
        return None


# Function to display data analysis section
def display_data_analysis_section(cursor):
    st.markdown("Data Analysis Section")

    # Time range selection
    time_range = st.selectbox(
        "Time Range:", ["Last 24 Hours", "Last 7 Days", "Last 30 Days", "All Time"]
    )

    # Granularity selection
    granularity = st.selectbox(
        "Granularity:", ["Hourly", "Daily", "Minute", "5 Minutes"]
    )  # Added Minute and 5 Minutes

    # Fetch available sensors
    cursor.execute("SELECT DISTINCT Sensor_ID FROM SensorsData;")
    sensors = cursor.fetchall()
    sensors = [sensor[0] for sensor in sensors]

    # Checkbox for selecting sensors to include
    selected_sensors = st.multiselect("Select Sensors to Include:", sensors)

    # Fetch data for analysis
    df = fetch_data_for_analysis(cursor, time_range, granularity, selected_sensors)
    if df is None:
        return

    # Display data
    st.write("Data for Analysis:")
    st.dataframe(df)

    # Plot data
    st.subheader("Data Visualization")
    if st.checkbox("Show Temperature Trends"):
        st.write("Temperature Trends")
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x="Time", y="Temperature")
        plt.xlabel("Time")
        plt.ylabel("Temperature")
        plt.title("Temperature Trends")
        st.pyplot()

    if st.checkbox("Show Humidity Trends"):
        st.write("Humidity Trends")
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x="Time", y="Humidity")
        plt.xlabel("Time")
        plt.ylabel("Humidity")
        plt.title("Humidity Trends")
        st.pyplot()


# Function to display alarms section
def display_alarms_section(cursor):
    pass


# Main function
def main():
    manage_session_state()

    st.set_option("deprecation.showPyplotGlobalUse", False)

    st.title("Urban Farm Monitoring")

    # Render navigation menu
    st.sidebar.title("Navigation Menu")
    pages = ["Home", "Sensors", "Data Analysis", "Alarms"]
    selected_page = st.sidebar.radio(
        "Go to", pages, index=pages.index(st.session_state.page)
    )

    # Rerun the app only when the selected page is different
    if selected_page != st.session_state.page:
        st.session_state.page = selected_page
        st.rerun()

    # Connect to the database
    conn = connect_to_db()
    while not conn:
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
