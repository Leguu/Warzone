#ifndef WARZONE_UTILS_H
#define WARZONE_UTILS_H

#include <vector>
#include <string>
using std::string;
using std::vector;

class Utils {
public:
  static std::string getInputString(const std::string &prompt);

  static int getInputInt(const std::string &prompt);

  static vector<std::string> *tokenizer(const string &s, char del);

  static string trim(const string &s);

  static string WHITESPACE;

  static bool isEqualLowercase(const string &a, const string &b);

  static void assertCondition(bool condition, const std::string message);
};

#endif //WARZONE_UTILS_H
