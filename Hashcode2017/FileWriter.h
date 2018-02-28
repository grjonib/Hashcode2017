#pragma once

#include "DataClasses.h"

#include <string>
#include <iostream>
#include <fstream>



class FileWriter
{
public:
	static void writeFile(std::string outputFileName, AssignmentData &data)
	{
		std::cout << "Writing to: " << outputFileName << std::endl;
		std::ofstream output;
		output.open(outputFileName);
		output << data.caches.size() << "\n";
		for (auto&& cache : data.caches)
		{
			output << cache.number;
			for (auto&& videoId : cache.videoId)
			{
				output << " " << videoId;
			}
			output << "\n";
		}
		output.close();

		return;
	}

private:
	// Disallow creating an instance of this object
	FileWriter() {}
};

