/// @brief Include to load KTX textures from files or memory.
/// @file gli/load_ktx.hpp

#pragma once

#include "texture.hpp"

namespace gli
{
	/// Loads a texture storage from KTX file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_ktx(char const * Path);

	/// Loads a texture storage from KTX file. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_ktx(std::string const & Filename);

	/// Loads a texture storage from KTX memory. Returns an empty storage in case of failure.
	///
	/// @param Path Path of the file to open including filaname and filename extension
	texture load_ktx(char const * Data, std::size_t Size);
}//namespace gli

#include "./core/load_ktx.inl"
