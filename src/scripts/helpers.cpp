#include "headers/helpers.h"

String generateRandomPassword(int length) {
  const char* charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
  String password = "";

  int setLength = strlen(charSet);

  for (int i = 0; i < length; i++) {
    password += charSet[random(0, setLength)];
  }

  return password;
}