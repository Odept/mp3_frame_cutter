#pragma once

#include <memory>
#include <vector>


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
		virtual unsigned		getFrameCount	() const = 0;
		virtual float			getLength		() const = 0;

		virtual Version			getVersion		() const = 0;
		virtual unsigned		getLayer		() const = 0;
		virtual unsigned		getBitrate		() const = 0;
		virtual bool			isVBR			() const = 0;
		virtual unsigned		getSamplingRate	() const = 0;
		virtual ChannelMode		getChannelMode	() const = 0;
		virtual Emphasis		getEmphasis		() const = 0;

		virtual size_t			getFrameOffset	(unsigned f_index) const = 0;
		virtual unsigned		getFrameSize	(unsigned f_index) const = 0;
		virtual float			getFrameTime	(unsigned f_index) const = 0;

		virtual void			serialize		(std::vector<unsigned char>& f_outStream) = 0;

		// Return the number of processed frames
		virtual unsigned		cut				(unsigned f_frame, unsigned f_count) = 0;
		virtual unsigned		truncate		(unsigned f_frames) = 0;

		virtual					~IStream		();
	};
}

