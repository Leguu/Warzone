#ifndef WARZONE_UTILS_H
#define WARZONE_UTILS_H

#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::getline;
using std::endl;

class Utils {
public:
  static string getInputString();

  static string getInputString(const string &prompt);

  static int getInputInt(const string &prompt);

  static int getInputInt(const string &prompt, bool cancelable);

  static vector<string> *tokenizer(const string &s, char del);

  static void assert(bool condition, const string &test);

  static string trim(const string &s);

  static string WHITESPACE;

  static const string inputPrompt;

  static string toLowercase(const string &a);

  static bool isEqualLowercase(const string &a, const string &b);

  class CancelledInputException : public std::runtime_error {
  public:
    explicit CancelledInputException();

    ~CancelledInputException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

  };
};

#endif //WARZONE_UTILS_H
