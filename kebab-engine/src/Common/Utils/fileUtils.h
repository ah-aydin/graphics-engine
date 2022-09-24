#pragma once

#include <string>
#include <vector>

int endsWith(const char* s, const char* part);

std::string readShaderFile(const char* fileName);

void printShaderSource(const char* text);

std::vector<char> readSPIRV(const std::string& filename);