#include "MeshLoader.h"

//std::string* split(std::string input) {
//	std::vector<std::string> strings;
//	
//}

MeshLoader::MeshLoader(std::string fileName)
{
	std::ifstream file(fileName);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			if (line[0] == 'v' && line[1] != 'n' && line[1] != 't') {

			}else if (line[0] == 'v' && line[1] == 'n') {

			}else if (line[0] == 'v' && line[1] == 't') {

			}else if (line[0] == 'f') {

			}
		}
	}
}


MeshLoader::~MeshLoader()
{
}
