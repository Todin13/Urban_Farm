#include <iostream>
#include <pqxx/pqxx>
#include "crow.h"
#include <sstream>
#include <json/json.h>
#include <iterator>

// Function prototype declaration
std::string getDashboardOverview();

// Function to query the database and retrieve dashboard overview data
std::string getDashboardOverview() {
    std::stringstream response;

    try {
        pqxx::connection conn("dbname=urbanfarm user=admin password=urbanfarm123 hostaddr=127.0.0.1 port=5433");
        pqxx::work txn(conn);

        pqxx::result result = txn.exec("SELECT COUNT(*) AS experiment_count FROM Experiment;"
                                        "SELECT COUNT(*) AS sensor_count FROM Sensors;"
                                        "SELECT COUNT(*) AS warning_count FROM Warnings;"
                                        "SELECT AVG(Temperature) AS avg_temperature, AVG(Humidity) AS avg_humidity FROM SensorsData;");

        int experiment_count = result[0][0].as<int>();
        int sensor_count = result[1][0].as<int>();
        int warning_count = result[2][0].as<int>();
        float avg_temperature = 0.0f;
        float avg_humidity = 0.0f;

        /for (auto field = std::begin(result[3][0]); field != std::end(result[3][0]); ++field) {
            // Check field name and extract value if it matches
            std::string field_name = field.name();
            if (field_name == "avg_temperature") {
                avg_temperature = field.as<float>();
            } else if (field_name == "avg_humidity") {
                avg_humidity = field.as<float>();
            }
        }


        // Build JSON response
        Json::Value data;
        data["experiment_count"] = experiment_count;
        data["sensor_count"] = sensor_count;
        data["warning_count"] = warning_count;
        data["avg_temperature"] = avg_temperature;
        data["avg_humidity"] = avg_humidity;

        // Convert JSON to string
        Json::StreamWriterBuilder builder;
        response << Json::writeString(builder, data);
    } catch (const std::exception &e) {
        std::cerr << "Error fetching dashboard data: " << e.what() << std::endl;
        response << "{}";
    }

    return response.str();
}

int main() {
    crow::SimpleApp app;

    // Route for the home page
    CROW_ROUTE(app, "/")([](){
        std::string dashboardOverview = getDashboardOverview();
        return R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Urban Farm Monitoring</title>
                <style>
                    /* CSS styles for layout */
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 0;
                    }
                    header {
                        background-color: #333;
                        color: #fff;
                        text-align: center;
                        padding: 10px 0;
                    }
                    nav {
                        background-color: #f4f4f4;
                        padding: 10px;
                    }
                    nav ul {
                        list-style-type: none;
                        margin: 0;
                        padding: 0;
                        text-align: center;
                    }
                    nav ul li {
                        display: inline;
                        margin-right: 10px;
                    }
                    section {
                        padding: 20px;
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                    }
                    table, th, td {
                        border: 1px solid #ddd;
                        padding: 8px;
                        text-align: left;
                    }
                    th {
                        background-color: #f2f2f2;
                    }
                </style>
            </head>
            <body>
                <header>
                    <h1>Urban Farm Monitoring</h1>
                </header>
                <nav>
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/sensors">Sensors</a></li>
                        <li><a href="/data">Data Analysis</a></li>
                        <li><a href="/alarms">Alarms</a></li>
                        <li><a href="/settings">Settings</a></li>
                    </ul>
                </nav>
                <section>
                    <section>
                    <h1>Urban Farm Monitoring</h1>
                    <h2>Dashboard Overview</h2>
                    <ul>)" + dashboardOverview + R"(</ul>
                </section>
                </section>
            </body>
            </html>
        )";
    });

    // Route for the Sensors page
    CROW_ROUTE(app, "/sensors")([](){
        return R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Sensors</title>
                <style>
                    /* CSS styles for layout */
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 0;
                    }
                    header {
                        background-color: #333;
                        color: #fff;
                        text-align: center;
                        padding: 10px 0;
                    }
                    nav {
                        background-color: #f4f4f4;
                        padding: 10px;
                    }
                    nav ul {
                        list-style-type: none;
                        margin: 0;
                        padding: 0;
                        text-align: center;
                    }
                    nav ul li {
                        display: inline;
                        margin-right: 10px;
                    }
                    section {
                        padding: 20px;
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                    }
                    table, th, td {
                        border: 1px solid #ddd;
                        padding: 8px;
                        text-align: left;
                    }
                    th {
                        background-color: #f2f2f2;
                    }
                </style>
            </head>
            <body>
                <header>
                    <h1>Sensors Section</h1>
                </header>
                <nav>
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/sensors">Sensors</a></li>
                        <li><a href="/data">Data Analysis</a></li>
                        <li><a href="/alarms">Alarms</a></li>
                        <li><a href="/settings">Settings</a></li>
                    </ul>
                </nav>
                <section>
                   <table>
                        <thead>
                            <tr>
                                <th>Sensor ID</th>
                                <th>Location</th>
                                <th>Temperature</th>
                                <th>Humidity</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td>S1</td>
                                <td>Greenhouse 1</td>
                                <td>24°C</td>
                                <td>65%</td>
                            </tr>
                            <tr>
                                <td>S2</td>
                                <td>Greenhouse 2</td>
                                <td>26°C</td>
                                <td>58%</td>
                            </tr>
                            <!-- Add more sensor data here -->
                        </tbody>
                    </table>
                </section>
            </body>
            </html>
        )";
    });

    // Route for the Data Analysis page
    CROW_ROUTE(app, "/data")([](){
        return R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Data Analysis</title>
                <style>
                    /* CSS styles for layout */
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 0;
                    }
                    header {
                        background-color: #333;
                        color: #fff;
                        text-align: center;
                        padding: 10px 0;
                    }
                    nav {
                        background-color: #f4f4f4;
                        padding: 10px;
                    }
                    nav ul {
                        list-style-type: none;
                        margin: 0;
                        padding: 0;
                        text-align: center;
                    }
                    nav ul li {
                        display: inline;
                        margin-right: 10px;
                    }
                    section {
                        padding: 20px;
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                    }
                    table, th, td {
                        border: 1px solid #ddd;
                        padding: 8px;
                        text-align: left;
                    }
                    th {
                        background-color: #f2f2f2;
                    }
                </style>
            </head>
            <body>
                <header>
                    <h1>Data Analysis Section</h1>
                </header>
                <nav>
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/sensors">Sensors</a></li>
                        <li><a href="/data">Data Analysis</a></li>
                        <li><a href="/alarms">Alarms</a></li>
                        <li><a href="/settings">Settings</a></li>
                    </ul>
                </nav>
                <section>
                    <p>[ Graph: Temperature Trends ]</p>
                    <p>[ Graph: Humidity Trends ]</p>
                    <p>Time Range: Last 24 Hours</p>
                    <p>Granularity: Hourly</p>
                </section>
            </body>
            </html>
        )";
    });

    // Route for the Alarms page
    CROW_ROUTE(app, "/alarms")([](){
        return R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Alarms</title>
                <style>
                    /* CSS styles for layout */
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 0;
                    }
                    header {
                        background-color: #333;
                        color: #fff;
                        text-align: center;
                        padding: 10px 0;
                    }
                    nav {
                        background-color: #f4f4f4;
                        padding: 10px;
                    }
                    nav ul {
                        list-style-type: none;
                        margin: 0;
                        padding: 0;
                        text-align: center;
                    }
                    nav ul li {
                        display: inline;
                        margin-right: 10px;
                    }
                    section {
                        padding: 20px;
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                    }
                    table, th, td {
                        border: 1px solid #ddd;
                        padding: 8px;
                        text-align: left;
                    }
                    th {
                        background-color: #f2f2f2;
                    }
                </style>
            </head>
            <body>
                <header>
                    <h1>Alarms Section</h1>
                </header>
                <nav>
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/sensors">Sensors</a></li>
                        <li><a href="/data">Data Analysis</a></li>
                        <li><a href="/alarms">Alarms</a></li>
                        <li><a href="/settings">Settings</a></li>
                    </ul>
                </nav>
                <section>
                    <p>Active Alarms:</p>
                    <ul>
                        <li>Sensor S3 - High Temperature</li>
                        <li>Sensor S8 - Low Humidity</li>
                        <!-- Add more alarms here -->
                    </ul>
                </section>
            </body>
            </html>
        )";
    });

    // Route for the Settings page
    CROW_ROUTE(app, "/settings")([](){
        return R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Settings</title>
                <style>
                    /* CSS styles for layout */
                    body {
                        font-family: Arial, sans-serif;
                        margin: 0;
                        padding: 0;
                    }
                    header {
                        background-color: #333;
                        color: #fff;
                        text-align: center;
                        padding: 10px 0;
                    }
                    nav {
                        background-color: #f4f4f4;
                        padding: 10px;
                    }
                    nav ul {
                        list-style-type: none;
                        margin: 0;
                        padding: 0;
                        text-align: center;
                    }
                    nav ul li {
                        display: inline;
                        margin-right: 10px;
                    }
                    section {
                        padding: 20px;
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                    }
                    table, th, td {
                        border: 1px solid #ddd;
                        padding: 8px;
                        text-align: left;
                    }
                    th {
                        background-color: #f2f2f2;
                    }
                </style>
            </head>
            <body>
                <header>
                    <h1>Settings Section</h1>
                </header>
                <nav>
                    <ul>
                        <li><a href="/">Home</a></li>
                        <li><a href="/sensors">Sensors</a></li>
                        <li><a href="/data">Data Analysis</a></li>
                        <li><a href="/alarms">Alarms</a></li>
                        <li><a href="/settings">Settings</a></li>
                    </ul>
                </nav>
                <section>
                    <p>System Settings:</p>
                    <ul>
                        <li>Alarm Thresholds</li>
                        <li>Data Storage Options</li>
                        <li>Notification Preferences</li>
                    </ul>
                </section>
            </body>
            </html>
        )";
    });

    app.port(1400).run();

    return 0;
}
