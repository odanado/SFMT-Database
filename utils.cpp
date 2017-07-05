#include "utils.h"
#include <sstream>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif

void createDir(const string& dir) {
#if defined _MSC_VER
    _mkdir(dir.c_str());
#elif defined __GNUC__
    mkdir(dir.c_str(), 0777);
#endif
}

string getFilename(const string& dirName, int i) {
    std::stringstream ss;
    ss << dirName << "/QR";
    ss << std::setfill('0') << std::setw(2) << i;
    ss << ".bin";
    return ss.str();
}
