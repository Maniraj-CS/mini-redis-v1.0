#include "cache/Persistence.hpp"
#include "cache/KeyValueStore.hpp"

#include <fstream>
#include <sstream>

Persistence::Persistence(const std::string &filename) : filename(filename) {};

void Persistence::logSet(const std::string &key, const std::string &value, int ttl)
{

	std::lock_guard<std::mutex> lock(mtx);

	std::ofstream file(filename, std::ios::app);
	if (!file)
		return;

	if (ttl <= 0)
	{
		file << "SET " << key << " " << value << " 0\n";
		return;
	}

	auto expiry = std::chrono::system_clock::now() + std::chrono::seconds(ttl);

	auto expiryTimestamp =
		std::chrono::duration_cast<std::chrono::seconds>(
			expiry.time_since_epoch())
			.count();

	file << "SET " << key << " " << value << " " << expiryTimestamp << "\n";
}

void Persistence::logDel(const std::string &key)
{

	std::lock_guard<std::mutex> lock(mtx);

	std::ofstream file(filename, std::ios::app);

	if (!file)
		return;

	file << "DEL " << key << "\n";
}

void Persistence::load(KeyValueStore &storage)
{

	std::ifstream file(filename);

	if (!file)
		return;

	std::string line;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);

		std::string command;

		ss >> command;

		if (command == "SET")
		{

			std::string key;
			std::string value;
			long long expiryTimestamp;

			if (!(ss >> key >> value >> expiryTimestamp))
			{
				continue;
			}

			/*
			If there extra argument peresent then skip the line
			*/
			std::string extra;

			if (ss >> extra)
			{
				continue;
			}

			/*
				If expiryTimstamp is zero its mean that there is no expiry time seted.
			*/
			if (expiryTimestamp == 0)
			{
				storage.setInternal(key, value, 0);
				continue;
			}

			auto now = std::chrono::system_clock::now();
			auto currentTimestamp =
				std::chrono::duration_cast<std::chrono::seconds>(
					now.time_since_epoch())
					.count();

			long long remainingTTL = expiryTimestamp - currentTimestamp;

			if (remainingTTL <= 0)
			{
				continue;
			}

			storage.setInternal(key, value, remainingTTL);
		}
		else if (command == "DEL")
		{

			std::string key;

			if (!(ss >> key))
				continue;

			/*
			If there extra argument peresent then skip the line
			*/

			std::string extra;

			if (ss >> extra)
			{
				continue;
			}

			storage.delInternal(key);
		}
		else if (command == "CLEAR")
		{
			storage.clearInternal();
		}
		else
		{
			// Invalid command, ignore
			continue;
		}
	}
}

void Persistence::logClear()
{

	std::lock_guard<std::mutex> lock(mtx);

	std::ofstream file(filename, std::ios::app);

	if (!file)
		return;

	file << "CLEAR\n";
}