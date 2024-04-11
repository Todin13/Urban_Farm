#include <iostream>
#include "crow.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "<h1>Urban Farm Monitoring</h1>"
               "<p>Dashboard Overview</p>"
               "<p>Metrics:</p>"
               "<ul>"
               "<li>Temperature: 25°C</li>"
               "<li>Humidity: 60%</li>"
               "<li>Sensors Online: 10/10</li>"
               "<li>Alarms: 2 Active</li>"
               "</ul>";
    });

    app.port(14300).multithreaded().run();

    return 0;
}
