#include "WarningReporter.h"
#include <cpprest/http_client.h>

void WarningReporter::report(const SensorData& data) {
    // Implement the logic to send a POST request with the anomaly warning
    web::http::client::http_client client(U("http://visualization-service"));
    web::json::value warningJson = JsonSerializer::serialize(data);
    client.request(web::http::methods::POST, U("/warning"), warningJson).then([](web::http::http_response response) {
        if (response.status_code() == web::http::status_codes::OK) {
            // Handle successful response
        } else {
            // Handle error
        }
    }).wait();
}