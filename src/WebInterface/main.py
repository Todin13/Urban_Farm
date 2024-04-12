import streamlit as st
import psycopg2
import os

# Database connection
def connect_to_db():
    conn = psycopg2.connect(
        dbname=os.environ.get("POSTGRES_DB"),
        user=os.environ.get("POSTGRES_USER"),
        password=os.environ.get("POSTGRES_PASSWORD"),
        host="localhost",
        port="5433"
    )
    return conn

# Function to display dashboard overview
def display_dashboard_overview():
    st.markdown("Dashboard Overview")
    # Fetch and display metrics from the database

# Function to display sensors section
def display_sensors_section():
    st.markdown("Sensors Section")
    # Fetch and display sensor data from the database

# Function to display data analysis section
def display_data_analysis_section():
    st.markdown("Data Analysis Section")
    # Display data analysis options and graphs

# Function to display alarms section
def display_alarms_section():
    st.markdown("Alarms Section")
    # Fetch and display active alarms from the database

# Main function
def main():
    st.title("Urban Farm Monitoring")

    # Render navigation menu
    st.sidebar.title("Navigation Menu")
    pages = ["Home", "Sensors", "Data Analysis", "Alarms"]
    choice = st.sidebar.radio("Go to", pages)

    # Connect to the database
    conn = connect_to_db()
    cursor = conn.cursor()

    # Display content based on navigation choice
    if choice == "Home":
        display_dashboard_overview()
    elif choice == "Sensors":
        display_sensors_section()
    elif choice == "Data Analysis":
        display_data_analysis_section()
    elif choice == "Alarms":
        display_alarms_section()

    # Close database connection
    cursor.close()
    conn.close()

if __name__ == "__main__":
    main()
