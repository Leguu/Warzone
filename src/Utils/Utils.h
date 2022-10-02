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

  static string trim(const string &s);

  static string WHITESPACE;

  static const string inputPrompt;

  static string toLowercase(const string &a);

  static bool isEqualLowercase(const string &a, const string &b);

  static void assertCondition(bool condition, const std::string message);

  class CancelledInputException : public std::runtime_error {
  public:
	explicit CancelledInputException();

	~CancelledInputException() override;
  };
};

#endif //WARZONE_UTILS_H
