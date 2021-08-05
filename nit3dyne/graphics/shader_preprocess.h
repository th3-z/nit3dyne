#ifndef NIT3DYNE_EX_SHADER_PREPROCESS_H
#define NIT3DYNE_EX_SHADER_PREPROCESS_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace n3d {

std::vector<std::string> strSplit(const std::string &str, const char delimiter);

std::string preprocessShader(const std::string &src);

}

#endif //NIT3DYNE_EX_SHADER_PREPROCESS_H
