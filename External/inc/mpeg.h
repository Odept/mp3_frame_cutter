#pragma once

#include <memory>


namespace MPEG
{
	enum class Version
	{
		v25			= 0,
		vReserved	= 1,
		v2			= 2, // ISO/IEC 13818-3
		v1			= 3	 // ISO/IEC 11172-3
	};

	enum class ChannelMode
	{
		Stereo		= 0,
		JointStereo	= 1,
		Dual		= 2,
		Mono		= 3
	};

	enum class Emphasis
	{
		None		= 0,
		_5015		= 1,
		Reserved	= 2,
		CCITJ17		= 3
	};


	class IStream
	{
	public:
		static std::shared_ptr<IStream>	create					(const unsigned char* f_data, size_t f_size);

		static size_t					calcFirstHeaderOffset	(const unsigned char* f_data, size_t f_size);
		static bool						verifyFrameSequence		(const unsigned char* f_data, size_t f_size);

		static bool						isIncompleteFrame		(const unsigned char* f_data, size_t f_size);

		static const std::string&		str						(Version f_version);
		static const std::string&		str						(ChannelMode f_mode);
		static const std::string&		str						(Emphasis f_emphasis);

	public:
		virtual bool			hasIssues		() const = 0;

		virtual size_t			getSize			() const = 0;
		virtual unsigned int	getFrameCount	() const = 0;
		virtual float			getLength		() const = 0;

		virtual Version			getVersion		() const = 0;
		virtual unsigned int	getLayer		() const = 0;
		virtual unsigned int	getBitrate		() const = 0;
		virtual bool			isVBR			() const = 0;
		virtual unsigned int	getSamplingRate	() const = 0;
		virtual ChannelMode		getChannelMode	() const = 0;
		virtual Emphasis		getEmphasis		() const = 0;

		virtual size_t			getFrameOffset	(unsigned int f_index) const = 0;
		virtual unsigned int	getFrameSize	(unsigned int f_index) const = 0;
		virtual float			getFrameTime	(unsigned int f_index) const = 0;

		// returns the number of deleted frames
		virtual unsigned int	truncate		(unsigned int f_frames) = 0;

		virtual					~IStream		();
	};
}

