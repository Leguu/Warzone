#ifndef WARZONE_UTILS_H
#define WARZONE_UTILS_H

#include <stdexcept>
#include <string>
#include <vector>

using std::getline;
using std::runtime_error;
using std::string;
using std::vector;

class Utils {
  public:
  static string getInputString();

  static string getInputString(const string &prompt);

  static int getInputInt(const string &prompt);

  static int getInputInt(const string &prompt, bool cancelable);

  static vector<string> tokenizer(const string &s, char del);

  static string trim(const string &s);

  static string WHITESPACE;

  static const string inputPrompt;

  static string toLowercase(const string &a);

  static bool isEqualLowercase(const string &a, const string &b);

  static int randomNumberInRange(int a, int b);

  static bool weightedBoolean(int percentage);

  static void assertCondition(bool condition, const std::string &message);

  class CancelledInputException : public runtime_error {
public:
    explicit CancelledInputException();

    ~CancelledInputException() override;
  };

  template<class T>
  static T *accessRandomElement(vector<T *> elements) {
    if (elements.empty()) {
      return nullptr;
    }
    int randomIndex = rand() % elements.size();
    return elements[randomIndex];
  }

  template<class T>
  static std::pair<T *, T *>
  accessRandomPair(vector<std::pair<T *, T *>> elements) {
    if (elements.empty()) {
      throw runtime_error("Error: can't access a random pair from an empty array!");
    }
    int randomIndex = rand() % elements.size();
    return elements[randomIndex];
  }

  private:
  inline Utils() = default;
};

#endif// WARZONE_UTILS_H
