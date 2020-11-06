#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"
#include <string>
#include <memory>

struct Texture : private NonCopyable, private NonMovable
{
	static std::unique_ptr<Texture> create(const std::string& path, const std::string& directory, const std::string& type);
	static std::unique_ptr<Texture> create(const std::string& fileName, const std::string& type);
	~Texture();

	void bind() const;

	const unsigned int ID;
	const std::string type;
	const std::string path;

private:
	Texture(unsigned int ID, const std::string& type, const std::string& path);
	Texture(unsigned int ID, const std::string& type);
};