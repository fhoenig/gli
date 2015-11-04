///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/sampler2d.inl
/// @date 2015-09-29 / 2015-09-29
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include "clear.hpp"
#include <glm/vector_relational.hpp>

namespace gli
{
	template <typename T, precision P>
	inline sampler2DArray<T, P>::sampler2DArray(texture_type const & Texture, gli::wrap Wrap, filter Mip, filter Min, texel_type const & BorderColor)
		: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
		, Texture(Texture)
		, Convert(detail::convert<texture_type, T, P>::call(this->Texture.format()))
		, BorderColor(BorderColor)
		, Filter(detail::get_filter<filter_type, detail::DIMENSION_2D, texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type, T>(Mip, Min, is_border(Wrap)))
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()));
		GLI_ASSERT((!std::numeric_limits<T>::is_iec559 && Mip == FILTER_NEAREST && Min == FILTER_NEAREST) || std::numeric_limits<T>::is_iec559);
	}

	template <typename T, precision P>
	inline typename sampler2DArray<T, P>::texture_type const & sampler2DArray<T, P>::operator()() const
	{
		return this->Texture;
	}

	template <typename T, precision P>
	inline typename sampler2DArray<T, P>::texel_type sampler2DArray<T, P>::texel_fetch(texelcoord_type const & TexelCoord, size_type layer, size_type Level) const
	{
		GLI_ASSERT(this->Convert.Fetch);
		return this->Convert.Fetch(this->Texture, TexelCoord, layer, 0, Level);
	}

	template <typename T, precision P>
	inline void sampler2DArray<T, P>::texel_write(texelcoord_type const & TexelCoord, size_type layer, size_type Level, texel_type const & Texel)
	{
		GLI_ASSERT(this->Convert.Write);
		this->Convert.Write(this->Texture, TexelCoord, layer, 0, Level, Texel);
	}

	template <typename T, precision P>
	inline void sampler2DArray<T, P>::clear(texel_type const & Color)
	{
		detail::clear<texture_type, T, P>::call(this->Texture, this->Convert.Write, Color);
	}

	template <typename T, precision P>
	inline typename sampler2DArray<T, P>::texel_type sampler2DArray<T, P>::texture_lod(samplecoord_type const & SampleCoord, size_type Layer, level_type Level) const
	{
		GLI_ASSERT(std::numeric_limits<T>::is_iec559);
		GLI_ASSERT(this->Convert.Fetch);
		GLI_ASSERT(this->Filter);

		samplecoord_type const SampleCoordWrap(this->Wrap(SampleCoord.x), this->Wrap(SampleCoord.y));

		return this->Filter(this->Texture, this->Convert.Fetch, SampleCoordWrap, Layer, size_type(0), Level, this->BorderColor);
	}

	template <typename T, precision P>
	inline void sampler2DArray<T, P>::generate_mipmaps(filter Min)
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(!is_compressed(this->Texture.format()));

		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level());
	}

	template <typename T, precision P>
	inline void sampler2DArray<T, P>::generate_mipmaps(size_type BaseLayer, size_type MaxLayer, size_type BaseLevel, size_type MaxLevel, filter Min)
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(!is_compressed(this->Texture.format()));
		GLI_ASSERT(this->Texture.max_level() >= MaxLevel);
		GLI_ASSERT(BaseLayer <= MaxLayer);
		GLI_ASSERT(BaseLevel <= MaxLevel);

		detail::generate_mipmaps_2d<texture_type, T, fetch_type, write_type, samplecoord_type, texel_type>(
			this->Texture, this->Convert.Fetch, this->Convert.Write, BaseLayer, MaxLayer, 0, 0, BaseLevel, MaxLevel, Min);
	}
}//namespace gli

