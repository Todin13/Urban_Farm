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
    // Handle POST requests
    request.extract_json().then([this](web::json::value requestBody) {
        SensorData sensorData = JsonSerializer::deserialize(requestBody);
        AnomalyDetector anomalyDetector;
        if (anomalyDetector.isAnomalous(sensorData)) {
            WarningReporter::report(sensorData);
        }
    }).wait();
    request.reply(web::http::status_codes::OK);
}