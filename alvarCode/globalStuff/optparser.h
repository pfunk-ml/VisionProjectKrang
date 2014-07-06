#ifndef OPTPARSER_H
#define OPTPARSER_H

#include "json/json.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

void parseJSONFile(const char* filename, Json::Value& config);

#endif // OPTPARSER_H
