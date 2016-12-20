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
		virtual size_t				getSize				() const					= 0;

		virtual unsigned			getMinorVersion		() const					= 0;
		virtual unsigned			getRevision			() const					= 0;

		virtual const std::string&	getTrack			() const					= 0;
		virtual void				setTrack			(const std::string& f_str)	= 0;

		virtual const std::string&	getDisc				() const					= 0;
		virtual void				setDisc				(const std::string& f_str)	= 0;

		virtual const std::string&	getBPM				() const					= 0;
		virtual void				setBPM				(const std::string& f_str)	= 0;


		virtual const std::string&	getTitle			() const					= 0;
		virtual void				setTitle			(const std::string& f_str)	= 0;

		virtual const std::string&	getArtist			() const					= 0;
		virtual void				setArtist			(const std::string& f_str)	= 0;

		virtual const std::string&	getAlbum			() const					= 0;
		virtual void				setAlbum			(const std::string& f_str)	= 0;

		virtual const std::string&	getAlbumArtist		() const					= 0;
		virtual void				setAlbumArtist		(const std::string& f_str)	= 0;

		virtual const std::string&	getYear				() const					= 0;
		virtual void				setYear				(const std::string& f_str)	= 0;

		virtual int					getGenreIndex		() const					= 0;
		virtual void				setGenreIndex		(unsigned f_index)			= 0;
		virtual const std::string&	getGenre			() const					= 0;
		virtual void				setGenre			(const std::string& f_text)	= 0;
		// True when "(index)text" is supplied and index != text
		virtual bool				isExtendedGenre		() const					= 0;

		virtual const std::string&	getComment			() const					= 0;
		virtual void				setComment			(const std::string& f_str)	= 0;


		virtual const std::string&	getComposer			() const					= 0;
		virtual void				setComposer			(const std::string& f_str)	= 0;

		virtual const std::string&	getPublisher		() const					= 0;
		virtual void				setPublisher		(const std::string& f_str)	= 0;

		virtual const std::string&	getOrigArtist		() const					= 0;
		virtual void				setOrigArtist		(const std::string& f_str)	= 0;

		virtual const std::string&	getCopyright		() const					= 0;
		virtual void				setCopyright		(const std::string& f_str)	= 0;

		virtual const std::string&	getURL				() const					= 0;
		virtual void				setURL				(const std::string& f_str)	= 0;

		virtual const std::string&	getEncoded			() const					= 0;
		virtual void				setEncoded			(const std::string& f_str)	= 0;

		// Complex metadata
		virtual const std::vector<unsigned char>&	getPictureData			() const	= 0;
		virtual const std::string&					getPictureDescription	() const	= 0;

		virtual	std::vector<std::string>			getUnknownFrames		() const	= 0;
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

