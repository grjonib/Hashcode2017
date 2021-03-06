// Hashcode2017.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "DataClasses.h"
#include "FileParser.h"
#include "FileWriter.h"

#include <string>
#include <vector>
#include <iterator>
#include <random>
#include <chrono>

void MyAwsomeAlgorithm(AssignmentData &data);
int CalculateScore(AssignmentData &data);

int main()
{	
	std::vector<std::string> dataFiles{
		//"datafiles/data.in",
		"datafiles/me_at_the_zoo.in",
		"datafiles/videos_worth_spreading.in",
		"datafiles/trending_today.in",
		//"datafiles/kittens.in",
	};

	std::chrono::high_resolution_clock clock;
	int totalScore{ 0 };

	//Loop through all the files
	for (auto&& fileName : dataFiles)
	{
		//Setup
		auto startTime = clock.now();
		AssignmentData data;
		FileParser fp{ data };
		fp.parseDataFile(fileName);

		//Run Algorithm
		MyAwsomeAlgorithm(data);

		auto scoreTime = clock.now();
		//Calculate score
		totalScore += CalculateScore(data);

		//Cleanup
		FileWriter::writeFile(fileName.substr(0, fileName.size() - 2) + "out", data);
		std::cout << "Processing " << fileName << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - startTime).count() << "(" << std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - scoreTime).count() << ") ms." << std::endl;
	}

	std::cout << "Total score for algo is " << totalScore << std::endl;
		
    return 0;
}

void MyAwsomeAlgorithm(AssignmentData &data)
{
	std::random_device rng;
	std::mt19937 urng(rng());

	for (int i = 0; i < data.numberOfCaches; i++)
	{
		int currentSize = 0;
		data.caches.emplace_back(Cache{ i });
		std::shuffle(data.videos.begin(), data.videos.end(), urng);
		for (int j = 0; j < data.videos.size(); j++)
		{
			if (currentSize + data.videos[j].size < data.cacheSize)
			{
				data.caches[i].videoId.emplace_back(data.videos[j].number);
				currentSize += data.videos[j].size;
			}

		}
		std::cout << "Cache " << i << " has " << data.caches[i].videoId.size() << " videos, and has " << currentSize << "Mb out of " << data.cacheSize << "." << std::endl;
	}
}

int CalculateScore(AssignmentData &data) {
	
	int totalSaved{ 0 };
	int totalRequest{ 0 };

	for (auto&& request : data.requests)
	{
		int videoID = request.videoNumber;
		int endpoint = request.endpointNumber;

		int lowestLatency = data.endpoints[endpoint].datacenterLatency;

		//Find all caches connected to this endpoint with this video.
		for (auto&& cache : data.endpoints[endpoint].caches)
		{
			if (std::find(data.caches[cache.number].videoId.begin(), data.caches[cache.number].videoId.end(), videoID) != data.caches[cache.number].videoId.end())
			{
				if (lowestLatency > cache.latency)
					lowestLatency = cache.latency;
			}
		}

		//Add request to score
		totalSaved += request.numberOfRequests * (data.endpoints[endpoint].datacenterLatency - lowestLatency);
		totalRequest += request.numberOfRequests;
	}

	std::cout << "Score is " << totalSaved / totalRequest << std::endl;

	return static_cast<int>( totalSaved / totalRequest * 1000 );

}

