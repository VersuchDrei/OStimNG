#include "Integrity.h"

#include "CheckSum.h"

#include <openssl/md5.h>

namespace Integrity {
    bool verifySceneIntegrity() {
        std::vector<std::string> files;
        for (auto& file : std::filesystem::recursive_directory_iterator{"Data\\SKSE\\Plugins\\OStim\\scenes"}) {
            if (file.is_directory()) {
                continue;
            }

            auto pathStr = file.path().string();
            if (pathStr.starts_with("Data\\SKSE\\Plugins\\OStim\\scenes\\OStim") && pathStr.ends_with(".json")) {
                files.push_back(pathStr);
            }
        }
        std::sort(files.begin(), files.end());

        std::ifstream integrity;
        integrity.open("Data\\OStim\\integrity\\scenes", std::ios::binary | std::ios::in);

        integrity.seekg(0, std::ios::end);
        long length = integrity.tellg();
        integrity.seekg(0, std::ios::beg);

        if (length / MD5_DIGEST_LENGTH != files.size()) {
            logger::warn("scene count mismatch: expected {}, encountered {}", length / MD5_DIGEST_LENGTH, files.size());
            return false;
        }

        char fileSum[MD5_DIGEST_LENGTH];
        std::string fileSumStr;

        for (std::string file : files) {
            integrity.read(fileSum, MD5_DIGEST_LENGTH);
            fileSumStr = std::string(fileSum, MD5_DIGEST_LENGTH);
            std::string checkSum = CheckSum::createCheckSum(file);

            if (fileSumStr != checkSum) {
                logger::warn("checksum mismatch for {}: expected {}, encountered {}", file, fileSumStr, checkSum);
                return false;
            }
        }

        return true;
    }
}