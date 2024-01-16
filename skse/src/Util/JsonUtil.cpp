#include "JsonUtil.h"

#include "StringUtil.h"

#include "Game/LocaleManager.h"

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

    void loadLowerString(json& json, std::string& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                value = json[propertyName];
                StringUtil::toLower(&value);
            } else {
                logger::warn("property '{}' of {} {} isn't a string", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void loadTranslatedString(json& json, std::string& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                value = LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(json[propertyName]));
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


    void consumeString(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                consumer(json[propertyName]);
            } else {
                logger::warn("property '{}' of {} {} isn't a string", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeLowerString(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                std::string value = json[propertyName];
                StringUtil::toLower(&value);
                consumer(value);
            } else {
                logger::warn("property '{}' of {} {} isn't a string", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeTranslatedString(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_string()) {
                consumer(LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(json[propertyName])));
            } else {
                logger::warn("property '{}' of {} {} isn't a string", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void consumeFloat(json& json, std::function<void(float)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_number()) {
                consumer(json[propertyName]);
            } else {
                logger::warn("property '{}' of {} {} isn't a number", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void consumeInt(json& json, std::function<void(int)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_number_integer()) {
                consumer(json[propertyName]);
            } else {
                logger::warn("property '{}' of {} {} isn't an integer", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
    
    void consumeBool(json& json, std::function<void(bool)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_boolean()) {
                consumer(json[propertyName]);
            } else {
                logger::warn("property '{}' of {} {} isn't a boolean", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }


    void consumeStringList(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_string()) {
                        consumer(element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeLowerStringList(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_string()) {
                        std::string value = element;
                        StringUtil::toLower(&value);
                        consumer(value);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeTranslatedStringList(json& json, std::function<void(std::string)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_string()) {
                        consumer(LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(element)));
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeFloatList(json& json, std::function<void(float)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_number()) {
                        consumer(element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a number", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeIntList(json& json, std::function<void(int)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_number_integer()) {
                        consumer(element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't an integer", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeBoolList(json& json, std::function<void(bool)> consumer, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_array()) {
                int index = 0;
                for (auto& element : json[propertyName]) {
                    if (element.is_boolean()) {
                        consumer(element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a boolean", index, propertyName, objectType, objectName);
                    }
                    index++;
                }
            } else {
                logger::warn("property '{}' of {} {} isn't a list", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }


    void consumeStringMap(json& json, std::function<void(std::string, std::string)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_string()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        consumer(keyVar, element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeLowerStringMap(json& json, std::function<void(std::string, std::string)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_string()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        std::string value = element;
                        StringUtil::toLower(&value);
                        consumer(keyVar, value);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeTranslatedStringMap(json& json, std::function<void(std::string, std::string)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_string()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        consumer(keyVar, LocaleManager::GetSingleton()->GetLocalization(static_cast<std::string>(element)));
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a string", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeFloatMap(json& json, std::function<void(std::string, float)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_number()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        consumer(keyVar, element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't a number", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeIntMap(json& json, std::function<void(std::string, int)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_number_integer()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        consumer(keyVar, element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't an integer", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }

    void consumeBoolMap(json& json, std::function<void(std::string, bool)> consumer, bool lowerKeys, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                for (auto& [key, element] : json[propertyName].items()) {
                    if (element.is_boolean()) {
                        std::string keyVar = key;
                        if (lowerKeys) {
                            StringUtil::toLower(&keyVar);
                        }
                        consumer(keyVar, element);
                    } else {
                        logger::warn("element {} of property '{}' of {} {} isn't an boolean", key, propertyName, objectType, objectName);
                    }
                }
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
    }
}