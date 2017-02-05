#pragma once

#include <stdint.h>

#include <vector>
#include <functional>

namespace TurboJpegIO
{
	enum PixelFormat {
		GRAY,
		RGB,
		RGBX
	};

	struct Arguments {
		int width, height;
		PixelFormat pixel_format;
	};

	class Writer
	{
	public:

		struct Context;
		Context* ctx;

		Writer();
		~Writer();

		void write(const Arguments& args,
			const uint8_t* data, size_t size, 
			const std::function<void(const uint8_t*, size_t)>& fn, 
			int jpeg_quality = 80);

	public:

		Writer(const Writer&) = delete;
		void operator=(const Writer&) = delete;
	};

	class Reader
	{
	public:

		struct Context;
		Context* ctx;

		Reader();
		~Reader();

		bool read(const uint8_t* data, size_t size, 
			PixelFormat request_pixelformat,
			const std::function<void(const Arguments& args,
				const uint8_t*, size_t)>& fn);

	protected:

		std::vector<uint8_t> buffer;

	public:

		Reader(const Reader&) = delete;
		void operator=(const Reader&) = delete;
	};
}
