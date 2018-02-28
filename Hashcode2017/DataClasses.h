#pragma once
#include <vector>

struct Video
{
	int number;
	int size;	
};

struct Cache
{
	int number;
	int latency;
	std::vector<int> videoId;
};

struct Endpoint
{	
	int number;
	int datacenterLatency;
	std::vector<Cache> caches;
};


struct Request
{
	int numberOfRequests;
	int videoNumber;
	int endpointNumber;
};

struct AssignmentData
{
	int numberOfVideos;
	int numberOfEndpoints;
	int numberOfRequestDescriptions;

	int numberOfCaches;
	int cacheSize;

	std::vector<Video> videos;
	std::vector<Endpoint> endpoints;
	std::vector<Request> requests;
	std::vector<Cache> caches;
};
