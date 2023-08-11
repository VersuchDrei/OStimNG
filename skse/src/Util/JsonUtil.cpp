#include "JsonUtil.h"

namespace JsonUtil {
    void loadString(json& json, std::string& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                value = json[propertyName];
            } else {
                logger::warn("property '{}' of {} {} isn't a string", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void loadFloat(json& json, float& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_number()) {
                value = json[propertyName];
            } else {
                logger::warn("property '{}' of {} {} isn't a number", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void loadInt(json& json, int& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_number_integer()) {
                value = json[propertyName];
            } else {
                logger::warn("property '{}' of {} {} isn't an integer", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void loadBool(json& json, bool& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_boolean()) {
                value = json[propertyName];
            } else {
                logger::warn("property '{}' of {} {} isn't a boolean", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
}