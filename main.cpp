#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

struct {
bool mean = false;
bool std = false;
bool min = false;
bool firstq = false;
bool median = false;
bool thirdq = false;
bool max = false;
} options;

enum stats {
MEAN,
STD,
MIN,
FIRSTQ,
MEDIAN,
THIRDQ,
MAX
};
static std::map<std::string, stats> statmap;

std::vector<std::vector<std::string>> data;
void reader(char* file) {
    std::ifstream reader;
    std::string line;
    data.reserve(11128);

    reader.open(file);
    getline(reader, line);
    int stat_count = stoi(line);
    for(int i = 0; i < stat_count; i++){
        getline(reader, line);
        std::cout <<line << std::endl;
        switch(statmap[line]){
            case(MEAN):
                options.mean = true;
                break;
            case(STD):
                options.std = true;
                break;
            case(MIN):
                options.min = true;
                break;
            case(FIRSTQ):
                options.firstq = true;
                break;
            case(MEDIAN):
                options.median = true;
                break;
            case(THIRDQ):
                options.thirdq = true;
                break;
            case(MAX):
                options.max = true;
                break;
        }

    }
    getline(reader, line);
    getline(reader, line, ',');
    int data_count = stoi(line);
    std::cout << data_count << std::endl;
    getline(reader, line);
    std::cout << line << std::endl;
    std::cout << statmap[line] << std::endl;
/*
    while (reader) {
        while (getline(reader, line))
        {
            std::stringstream ss(line);
            std::vector<std::string> linevec;
	while (getline(ss, line, ','))
            {
                linevec.push_back(std::move(line));
            }
            data.push_back(std::move(linevec));


        }
    }
*/
    reader.close();
    data.shrink_to_fit();
    return;
}


int main(int argc, char** argv) {
   reader(argv[1]);
    

	return 0;
}
