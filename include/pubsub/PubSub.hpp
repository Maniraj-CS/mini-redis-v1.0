
#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <unordered_set>

#include "ClientManager.hpp"

class PubSub {
	private:

	std::unordered_map<std::string , std::unordered_set<int>> channels;

	std::mutex mtx;

	ClientManager& clientManager;


	public:

	PubSub(ClientManager& manager);

	void subscribe(const std::string &channel , int clientID);
	void unsubscribe(const std::string &channel , int clientID);

	std::unordered_set<int> publish(const std::string &channel , const std::string &message);
	
};
