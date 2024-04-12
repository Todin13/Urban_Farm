#include "RestServer.h"
#include "AnomalyDetector.h"
#include "JsonSerializer.h"
#include "WarningReporter.h"

RestServer::RestServer(unsigned int port, DatabaseConnector& dbConnector, MetricsCollector& metricsCollector)
    : dbConnector(dbConnector), metricsCollector(metricsCollector),
      listener(web::http::uri_builder(U("http://localhost")).set_port(port).to_uri()) {
    listener.support(web::http::methods::GET, std::bind(&RestServer::handleGet, this, std::placeholders::_1));
    listener.support(web::http::methods::POST, std::bind(&RestServer::handlePost, this, std::placeholders::_1));
}

void RestServer::start() {
    listener.open().wait();
}

void RestServer::stop() {
    listener.close().wait();
}

void RestServer::handleGet(web::http::http_request request) {
    // Handle GET requests
}

void RestServer::handlePost(web::http::http_request request) {
    request.extract_json().then([this](web::json::value requestBody) {
        try {
            // Start a transaction
            pqxx::work txn(dbConnector.getConnection());

            SensorData sensorData = JsonSerializer::deserialize(requestBody);
            AnomalyDetector anomalyDetector(dbConnector);
            
            // Pass the transaction to the isAnomalous method
            if (anomalyDetector.isAnomalous(sensorData, txn)) {
                WarningReporter::report(sensorData);
                txn.commit();  // Commit the transaction if no exceptions
            } else {
                txn.abort();  // Rollback if not anomalous or in case of other criteria
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to analyze sensor data: " << e.what() << std::endl;
            // Consider how to handle HTTP response in case of failure
        }
    }).then([request]() {
        // Always reply to the HTTP request
        request.reply(web::http::status_codes::OK);
    });
}
