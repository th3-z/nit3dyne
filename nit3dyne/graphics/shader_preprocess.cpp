#include "shader_preprocess.h"

namespace n3d {

// TODO: move to shader
std::string SHADERS_PATH = "shaders/";

// TODO: Move to utils/string
std::vector<std::string> strSplit(const std::string &str, const char delimiter) {
    std::vector<std::string> out;
    auto first = std::begin(str);
    auto next = std::find(first, std::end(str), delimiter);

    for (; next != std::end(str); next = std::find(++first, std::end(str), delimiter)) {
        out.emplace_back(first, next);
        first = next;
    }

    // Input did not end with newline
    if (first != std::end(str))
        out.emplace_back(first, std::end(str));

    return out;
}

// TODO: Move to shader, also a lot of expensive copying going on here
std::string preprocessShader(const std::string &src) {
    std::string out;

    for (auto line : strSplit(src, '\n')) {
        auto words = strSplit(line, ' ');
        if (words.size() > 1 && words[0] == "#include") {
            auto &fName = words[1];
            // Remove quotes, and anything after closing " i.e. \n
            fName.erase(remove(fName.begin(), fName.end(), '\"'), fName.end());

            std::string incSrc;
            std::ifstream incFile;
            std::stringstream incStream;
            incFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                incFile.open(SHADERS_PATH + fName);
                incStream << incFile.rdbuf();
                incSrc = incStream.str();
                out += incSrc;
            } catch (std::ifstream::failure &e) {
                std::cout << "Shader include read error: " << fName << std::endl;
            }
        } else {
            out += line + '\n';
        }
    }

    std::cout << out << std::endl;
    return out;
}

}