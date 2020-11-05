#pragma once

#include "NonCopyable.h"
#include <memory>
#include <string>

class Texture : private NonCopyable
{
public:
	static std::unique_ptr<Texture> create(const char* path, const std::string& directory);

	unsigned int getID() const;
	const std::string& getType();
	const std::string& getPath();

private:
	Texture(unsigned int ID, const std::string& type, const std::string& path);
	~Texture();
	
	const unsigned int m_ID;
	const std::string m_type;
	const std::string m_path;
};