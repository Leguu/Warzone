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

  static void assert(bool condition, const std::string &test);

  static string trim(const string &s);

  static string WHITESPACE;

  static const string inputPrompt;

  static bool isEqualLowercase(const string &a, const string &b);

  class CancelledInputException : public std::runtime_error {
  public:
    explicit CancelledInputException();

    ~CancelledInputException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

  };
};

#endif //WARZONE_UTILS_H
