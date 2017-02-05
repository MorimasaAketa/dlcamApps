#include "TurboJpegIO.h"

#include <iostream>

#include "turbojpeg.h"

struct TurboJpegIO::Writer::Context {
	tjhandle handle;
};

TurboJpegIO::Writer::Writer()
	: ctx(new Context)
{
	ctx->handle = tjInitCompress();
}

TurboJpegIO::Writer::~Writer()
{
	tjDestroy(ctx->handle);
	delete ctx;
	ctx = nullptr;
}

void TurboJpegIO::Writer::write(const Arguments& args,
	const uint8_t* data, size_t size, 
	const std::function<void(const uint8_t*, size_t)>& fn, 
	int jpeg_quality)
{
	int pitch = 0, flags = 0, jpegsubsamp = 0;

	int pixel_format;
	int num_channels;

	switch (args.pixel_format) {
		case GRAY:
		{
			pixel_format = TJPF_GRAY;
			num_channels = 1;
			break;
		}
		case RGB:
		{
			pixel_format = TJPF_RGB;
			num_channels = 3;
			break;
		}
		case RGBX:
		{
			pixel_format = TJPF_RGBX;
			num_channels = 4;
			break;
		}
	}

	unsigned int bytesPerLine = num_channels *  args.width;

	unsigned char* out_data = NULL;
	unsigned long out_size = 0;

	int ok = tjCompress2(ctx->handle, (unsigned char*)data,
		args.width, bytesPerLine,
		args.height,
		pixel_format,
		&out_data, &out_size,
		jpegsubsamp, jpeg_quality, flags);

	if (ok == 0)
	{
		fn((const uint8_t*)out_data, out_size);
	}
	else
	{
		std::cerr << tjGetErrorStr() << std::endl;
	}

	tjFree(out_data);
}

//////////////////////////////////////////////////////////////////////////

struct TurboJpegIO::Reader::Context {
	tjhandle handle;
};

TurboJpegIO::Reader::Reader()
	: ctx(new Context)
{
	ctx->handle = tjInitDecompress();
}

TurboJpegIO::Reader::~Reader()
{
	tjDestroy(ctx->handle);
	delete ctx;
	ctx = nullptr;
}

bool TurboJpegIO::Reader::read(const uint8_t* data, size_t size, 
	PixelFormat request_pixelformat,
	const std::function<void(const Arguments& args, const uint8_t*, size_t)>& fn)
{
	int width = 0, height = 0;
	int jpegSubsamp, jpegColorspace;
	int ok;
	
	ok = tjDecompressHeader3(ctx->handle, (unsigned char*)data, size, &width, &height, &jpegSubsamp, &jpegColorspace);
	if (ok != 0)
	{
		std::cerr << tjGetErrorStr() << std::endl;
		return false;
	}

	int bpp = 0;
	int pixel_format = 0;

	switch (request_pixelformat)
	{
		case GRAY:
		{
			pixel_format = TJPF_GRAY;
			bpp = 1;
			break;
		}
		case RGB:
		{
			pixel_format = TJPF_RGB;
			bpp = 3;
			break;
		}
		case RGBX:
		{
			pixel_format = TJPF_RGBX;
			bpp = 4;
			break;
		}
	}

	int N = width * height * bpp;

	if (buffer.size() < N)
		buffer.resize(N);
	
	ok = tjDecompress2(ctx->handle, (unsigned char*)data, size, (unsigned char*)buffer.data(), 
		width, width * bpp, height, pixel_format, 0);
	if (ok != 0)
	{
		std::cerr << tjGetErrorStr() << std::endl;
		return false;
	}

	Arguments args;
	args.width = width;
	args.height = height;
	args.pixel_format = request_pixelformat;
	fn(args, buffer.data(), N);

	return true;
}

