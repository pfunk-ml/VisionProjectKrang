#include "optparser.h"
using namespace std;

// Parses a JSON file

void parseJSONFile(const char* filename, Json::Value& config)
{
    Json::Reader configParser;
    cout << filename << endl;
    ifstream file(filename);
    string config_string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    bool parsingSuccessful = configParser.parse(config_string, config);
    if (!parsingSuccessful)
    {
        cout << "Failed to parse JSON file: \n" << filename << endl << configParser.getFormattedErrorMessages();
        return;
    }

    cout << "Parse Successful" << endl;
    return;
}
