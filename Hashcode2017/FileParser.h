#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class FileParser
{
public:
	FileParser(AssignmentData &data) : data(data){};
	~FileParser() = default;
	
	void parseDataFile(std::string filename)
	{
		std::ifstream infile(filename);

		std::string line;

		//Parsing first line
		std::getline(infile, line);
		parseFirstLine(line);

		//Parsing second line
		std::getline(infile, line);
		parseVideoSizes(line);

		for (int endpoint = 0; endpoint < data.numberOfEndpoints; endpoint++)
		{
			std::getline(infile, line);
			int numberOfConnectedCaches = parseEndpointLine(line);

			for (int j = 0; j < numberOfConnectedCaches; j++)
			{
				std::getline(infile, line);
				addCachesToEndpoint(line);
			}
		}

		while (std::getline(infile, line))
		{
			addRequestLine(line);
		}
	}

private:

	AssignmentData &data;

	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	void parseFirstLine(std::string &line)
	{
		auto elems = split(line, ' ');

		if (elems.size() != 5)
		{
			std::cout << "FATAL ERROR, wrong number of parameters in first line" << std::endl;
			return;
		}

		data.numberOfVideos = std::stoi(elems[0]);
		data.numberOfEndpoints = std::stoi(elems[1]);
		data.numberOfRequestDescriptions = std::stoi(elems[2]);
		data.numberOfCaches = std::stoi(elems[3]);
		data.cacheSize = std::stoi(elems[4]);

	}

	void parseVideoSizes(std::string &line)
	{
		auto elems = split(line, ' ');

		if (elems.size() != data.numberOfVideos)
		{
			std::cout << "FATAL ERROR, wrong number of videos in secondline" << std::endl;
			return;
		}

		for (int i = 0; i < data.numberOfVideos; i++)
		{
			data.videos.emplace_back(Video{ i, std::stoi(elems[i]) });
		}

	}

	int parseEndpointLine(std::string &line)
	{
		auto elems = split(line, ' ');

		if (elems.size() != 2)
		{
			std::cout << "FATAL ERROR, endpoint line should be 2 elements" << std::endl;
			return 0;
		}

		data.endpoints.emplace_back(Endpoint{ (int)data.endpoints.size(), std::stoi(elems[0]) });


		return std::stoi(elems[1]);
	}

	void addCachesToEndpoint(std::string &line)
	{
		auto elems = split(line, ' ');

		if (elems.size() != 2)
		{
			std::cout << "FATAL ERROR, latency line should be 2 elements" << std::endl;
			return;
		}

		data.endpoints.back().caches.emplace_back(Cache{ std::stoi(elems[0]), std::stoi(elems[1]) });

	}

	void addRequestLine(std::string &line)
	{
		auto elems = split(line, ' ');

		if (elems.size() != 3)
		{
			std::cout << "FATAL ERROR, request line should be 3 elements" << std::endl;
			return;
		}

		data.requests.emplace_back(Request{ std::stoi(elems[2]), std::stoi(elems[0]), std::stoi(elems[1]) });

	}
};