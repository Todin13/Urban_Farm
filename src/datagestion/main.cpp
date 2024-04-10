#include <iostream>
#include "crow.h"

int main() {
    crow::SimpleApp app;

    // Define a route for POST requests to '/post' endpoint
    CROW_ROUTE(app, "/post")
    .methods("POST"_method)
    ([](const crow::request& req) {
        // Extract the JSON payload from the request
        auto json_payload = crow::json::load(req.body);

        if (!json_payload) {
            // JSON parsing failed
            return crow::response(400, "Invalid JSON payload");
        }

        // Process the JSON payload (example: echo back the received data)
        return crow::response(200, json_payload.dump());
    });

    // Run the server
    app.port(8080).multithreaded().run();

    return 0;
}
