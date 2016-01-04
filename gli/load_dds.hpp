/// @brief Include to load DDS textures from files or memory.
/// @file gli/load_dds.hpp

#pragma once

#include "texture.hpp"

namespace gli
{
	/// Loads a texture storage from DDS file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_dds(char const * Path);

	/// Loads a texture storage from DDS file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_dds(std::string const & Filename);

	/// Loads a texture storage from DDS memory. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_dds(char const * Data, std::size_t Size);
}//namespace gli

#include "./core/load_dds.inl"
