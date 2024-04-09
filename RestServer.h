#ifndef RESTSERVER_H
#define RESTSERVER_H

#include "DatabaseConnector.h"
#include "MetricsCollector.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

class RestServer {
public:
    RestServer(unsigned int port, DatabaseConnector& dbConnector, MetricsCollector& metricsCollector);
    void start();
    void stop();

private:
    web::http::experimental::listener::http_listener listener;
    DatabaseConnector& dbConnector;
    MetricsCollector& metricsCollector;

    void handleGet(web::http::http_request request);
    void handlePost(web::http::http_request request);
};

#endif // RESTSERVER_H