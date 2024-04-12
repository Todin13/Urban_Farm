#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "SensorData.h"
#include <cpprest/json.h>

class JsonSerializer {
public:
    static SensorData deserialize(const web::json::value& jsonValue);
    static web::json::value serialize(const SensorData& data);
};

#endif // JSONSERIALIZER_H