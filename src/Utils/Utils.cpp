#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

#include "Utils.h"

const string Utils::inputPrompt = ">";

string Utils::getInputString(const string &prompt) {
  std::cout << prompt << std::endl;
  return Utils::getInputString();
}

int Utils::getInputInt(const string &prompt, bool cancelable) {
  std::cout << prompt;
  if (cancelable) {
	std::cout << " Type in \"cancel\" to cancel this input.";
  }
  std::cout << std::endl;
  int value;

  while (true) {
	auto input = getInputString();

	if (cancelable && Utils::isEqualLowercase(input, "cancel")) {
	  throw CancelledInputException();
	}

	try {
	  value = stoi(input);
	  break;
	} catch (std::invalid_argument &e) {
	  std::cout << "Your input has to be a number!" << std::endl;
	  continue;
	}
  }

  return value;
}

int Utils::getInputInt(const string &prompt) {
  return getInputInt(prompt, false);
}

std::vector<string> Utils::tokenizer(const string &s, char del) {
  auto vec = std::vector<string>();
  auto ss = std::stringstream(Utils::trim(s));
  string word;
  while (!ss.eof()) {
	getline(ss, word, del);
	vec.push_back(word);
  }
  return vec;
}

string Utils::WHITESPACE = " \n\r\t\f\v";

string ltrim(const string &s) {
  size_t start = s.find_first_not_of(Utils::WHITESPACE);
  return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string &s) {
  size_t end = s.find_last_not_of(Utils::WHITESPACE);
  return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(const string &s) { return rtrim(ltrim(s)); }

bool Utils::isEqualLowercase(const string &a, const string &b) {
  return toLowercase(a) == toLowercase(b);
}

string Utils::getInputString() {
  std::cout << inputPrompt << std::flush;
  string input;
  getline(std::cin, input);
  return input;
}

string Utils::toLowercase(const string &a) {
  auto aStr = a;
  transform(aStr.begin(), aStr.end(), aStr.begin(), ::tolower);
  return aStr;
}

void Utils::assertCondition(bool condition, const std::string &message) {
  if (!condition) {
	throw std::runtime_error(message);
  }
}

int Utils::randomNumberInRange(int a, int b) {
  //  std::default_random_engine generator;
  //  std::uniform_int_distribution<int> distribution(a,  b));
  //  return distribution(generator);
  int range = (b - a + 1);
  range += (range == 0) ? 1 : 0;
  return rand() % range + a;
}

bool Utils::weightedBoolean(int percentage) {
  return randomNumberInRange(1, 100) <= percentage;
}

Utils::CancelledInputException::CancelledInputException()
	: runtime_error("Input was cancelled!") {}
Utils::CancelledInputException::~CancelledInputException() = default;
