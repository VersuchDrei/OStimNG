#pragma once

namespace StringUtil {
    void toLower(std::string* string);
    void toLower(std::vector<std::string>* strings);
    std::vector<std::string> toTagVector(std::string string);
    std::vector<std::vector<std::string>> toTagMatrix(std::string string);
    std::string toTagCSV(std::vector<std::string> vector);
}