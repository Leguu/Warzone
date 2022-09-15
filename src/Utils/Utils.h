#ifndef WARZONE_UTILS_H
#define WARZONE_UTILS_H


#include <string>

class Utils {
public:
    static std::string getInputString(const std::string &prompt);

    static int getInputInt(const std::string &prompt);
};


#endif //WARZONE_UTILS_H
