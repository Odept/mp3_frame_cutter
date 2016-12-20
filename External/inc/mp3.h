#pragma once

#include <memory> // shared_ptr


namespace MPEG
{
	class IStream;
}
namespace Tag
{
	class IID3v1;
	class IID3v2;
	class IAPE;
	class ILyrics;
}


class IMP3
{
public:
	static std::shared_ptr<IMP3> create(const unsigned char* f_data, size_t f_size);
	static std::shared_ptr<IMP3> create(const std::string& f_path);

	virtual std::shared_ptr<MPEG::IStream>	mpegStream		() const = 0;
	virtual std::shared_ptr<Tag::IID3v1>	tagID3v1		() const = 0;
	virtual std::shared_ptr<Tag::IID3v2>	tagID3v2		() const = 0;
	virtual std::shared_ptr<Tag::IAPE>		tagAPE			() const = 0;
	virtual std::shared_ptr<Tag::ILyrics>	tagLyrics		() const = 0;

	virtual unsigned						mpegStreamOffset() const = 0;
	virtual unsigned						tagID3v1Offset	() const = 0;
	virtual unsigned						tagID3v2Offset	() const = 0;
	virtual unsigned						tagAPEOffset	() const = 0;
	virtual unsigned						tagLyricsOffset	() const = 0;

	virtual bool							hasWarnings		() const = 0;

	virtual bool							serialize		(const std::string& f_path) = 0;

	virtual ~IMP3();

public:
	class exception : public std::exception
	{
	public:
		const char* what() const noexcept override = 0;
	};
};

