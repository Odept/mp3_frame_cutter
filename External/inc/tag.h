#pragma once


#include <vector>
#include <string>
#include <memory>


namespace Tag
{
	class ISerialize
	{
	public:
		virtual void serialize(std::vector<unsigned char>& f_outStream) = 0;

		virtual ~ISerialize();
	};


	class IID3v1 : public ISerialize
	{
	public:
		static size_t					size	();
		static size_t					getSize	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<IID3v1>	create	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<IID3v1>	create	();

	public:
		virtual size_t				getSize				() const					= 0;

		virtual bool				isV11				() const					= 0;

		virtual const std::string&	getTitle			() const					= 0;
		virtual void				setTitle			(const std::string& f_str)	= 0;

		virtual const std::string&	getArtist			() const					= 0;
		virtual void				setArtist			(const std::string& f_str)	= 0;

		virtual const std::string&	getAlbum			() const					= 0;
		virtual void				setAlbum			(const std::string& f_str)	= 0;

		virtual const std::string&	getYear				() const					= 0;
		virtual void				setYear				(const std::string& f_str)	= 0;

		virtual const std::string&	getComment			() const					= 0;
		virtual void				setComment			(const std::string& f_str)	= 0;

		virtual unsigned			getTrack			() const					= 0;
		virtual void				setTrack			(unsigned f_track)			= 0;

		virtual unsigned			getGenreIndex		() const					= 0;
		virtual void				setGenreIndex		(unsigned f_index)			= 0;
	};


	class IID3v2 : public ISerialize
	{
	public:
		static size_t					getSize	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<IID3v2>	create	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<IID3v2>	create	();

	public:
		virtual size_t				getSize				() const										= 0;

		virtual unsigned			getMinorVersion		() const										= 0;
		virtual unsigned			getRevision			() const										= 0;

		// ID3v2 tag can have multiple frames of the same type
		virtual unsigned			getTrackCount		() const										= 0;
		virtual const std::string&	getTrack			(unsigned f_index) const						= 0;
		virtual void				setTrack			(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getDiscCount		() const										= 0;
		virtual const std::string&	getDisc				(unsigned f_index) const						= 0;
		virtual void				setDisc				(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getBPMCount			() const										= 0;
		virtual const std::string&	getBPM				(unsigned f_index) const						= 0;
		virtual void				setBPM				(unsigned f_index, const std::string& f_str)	= 0;


		virtual unsigned			getTitleCount		() const										= 0;
		virtual const std::string&	getTitle			(unsigned f_index) const						= 0;
		virtual void				setTitle			(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getArtistCount		() const										= 0;
		virtual const std::string&	getArtist			(unsigned f_index) const						= 0;
		virtual void				setArtist			(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getAlbumCount		() const										= 0;
		virtual const std::string&	getAlbum			(unsigned f_index) const						= 0;
		virtual void				setAlbum			(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getAlbumArtistCount	() const										= 0;
		virtual const std::string&	getAlbumArtist		(unsigned f_index) const						= 0;
		virtual void				setAlbumArtist		(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getYearCount		() const										= 0;
		virtual const std::string&	getYear				(unsigned f_index) const						= 0;
		virtual void				setYear				(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getGenreCount		() const										= 0;
		virtual int					getGenreIndex		(unsigned f_index) const						= 0;
		virtual void				setGenreIndex		(unsigned f_index, unsigned f_genre_index)		= 0;
		virtual const std::string&	getGenre			(unsigned f_index) const						= 0;
		virtual void				setGenre			(unsigned f_index, const std::string& f_text)	= 0;
		// True when "(index)text" is supplied and index != text
		virtual bool				isExtendedGenre		(unsigned f_index) const						= 0;

		virtual unsigned			getCommentCount		() const										= 0;
		virtual const std::string&	getComment			(unsigned f_index) const						= 0;
		virtual void				setComment			(unsigned f_index, const std::string& f_str)	= 0;


		virtual unsigned			getComposerCount	() const										= 0;
		virtual const std::string&	getComposer			(unsigned f_index) const						= 0;
		virtual void				setComposer			(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getPublisherCount	() const										= 0;
		virtual const std::string&	getPublisher		(unsigned f_index) const						= 0;
		virtual void				setPublisher		(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getOrigArtistCount	() const										= 0;
		virtual const std::string&	getOrigArtist		(unsigned f_index) const						= 0;
		virtual void				setOrigArtist		(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getCopyrightCount	() const										= 0;
		virtual const std::string&	getCopyright		(unsigned f_index) const						= 0;
		virtual void				setCopyright		(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getURLCount			() const										= 0;
		virtual const std::string&	getURL				(unsigned f_index) const						= 0;
		virtual void				setURL				(unsigned f_index, const std::string& f_str)	= 0;

		virtual unsigned			getEncodedCount		() const										= 0;
		virtual const std::string&	getEncoded			(unsigned f_index) const						= 0;
		virtual void				setEncoded			(unsigned f_index, const std::string& f_str)	= 0;

		// Complex metadata
		virtual unsigned							getPictureCount			() const					= 0;
		virtual const std::vector<unsigned char>&	getPictureData			(unsigned f_index) const	= 0;
		virtual const std::string&					getPictureDescription	(unsigned f_index) const	= 0;

		virtual	std::vector<std::string>			getUnknownFrames		() const					= 0;
	};


	class IAPE : public ISerialize
	{
	public:
		static size_t					getSize	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<IAPE>	create	(const unsigned char* f_data, size_t f_size);

		virtual size_t					getSize	() const	= 0;
	};


	class ILyrics : public ISerialize
	{
	public:
		static size_t					getSize	(const unsigned char* f_data, size_t f_size);
		static std::shared_ptr<ILyrics>	create	(const unsigned char* f_data, size_t f_size);

		virtual size_t					getSize	() const	= 0;
	};


	const std::string&	genre(unsigned f_index);
	int					genre(const std::string& f_text);
}

