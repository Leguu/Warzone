#include <iostream>
#include <sstream>
#include "Utils.h"

const auto inputPrompt = ">";

std::string Utils::getInputString(const std::string &prompt) {
  std::cout << prompt << std::endl;
  std::cout << inputPrompt << std::flush;
  std::string input;
  std::cin >> input;
  return input;
}

int Utils::getInputInt(const std::string &prompt) {
  std::cout << prompt << std::endl;
  std::string input;
  int value;
  while (true) {
	std::cout << inputPrompt << std::flush;
	std::cin >> input;
	try {
	  value = std::stoi(input);
	  break;
	} catch (std::invalid_argument &e) {
	  std::cout << "Your input has to be a number!" << std::endl;
	  continue;
	}
  }
  return value;
}

vector<std::string> *Utils::tokenizer(const string &s, char del) {
  auto vec = new vector<string>();
  std::stringstream ss(s);
  string word;
  while (!ss.eof()) {
	getline(ss, word, del);
	vec->push_back(word);
  }
  return vec;
}

string Utils::WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(Utils::WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(Utils::WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string Utils::trim(const string &s) {
  return rtrim(ltrim(s));
}

bool Utils::isEqualLowercase(const string &a, const string &b) {
  auto aStr = a;
  auto bStr = b;
  std::transform(aStr.begin(), aStr.end(), aStr.begin(), ::tolower);
  std::transform(bStr.begin(), bStr.end(), bStr.begin(), ::tolower);
  return aStr == bStr;
}

void Utils::assertCondition(bool condition, const std::string message) {
  if (!condition) {
	throw std::runtime_error(message);
  }
}