#include "External/inc/mp3.h"
#include "External/inc/mpeg.h"
#include "External/inc/tag.h"

#include "commands.h"

#include "common.h"


static bool g_verbose = true;
#define VERBOSE(msg) if(g_verbose) LOG(msg)


// ====================================
// find test -name "*.mp3" -print0 | xargs -0 -I{} ./mp3_cut -i mpeg {}
static const uint s_captionWidth = 16;

using tag_frame_count_getter_t  = unsigned              (Tag::IID3v2::*)() const;
using tag_frame_getter_t        = const std::string&    (Tag::IID3v2::*)(unsigned f_index) const;
using tag_genre_index_getter_t  = int                   (Tag::IID3v2::*)(unsigned f_index) const;

static void printSeparator(bool& f_ioFirstFlag);
static std::string makeAlignedCaption(uint f_width, const std::string& f_name, int f_index = -1);
static void printFrames(const std::string& f_name, const Tag::IID3v2& f_tag, tag_frame_count_getter_t f_pfnCount, tag_frame_getter_t f_pfnGetter, tag_genre_index_getter_t f_pfnGenreIndex);


bool CmdInfo::exec() const
{
	std::shared_ptr<IMP3> mp3;
	try
	{
		VERBOSE("Outputting info for \"" << m_pathIn << '"');
		mp3 = IMP3::create(m_pathIn);
	}
	catch(IMP3::exception& e)
	{
		ERROR(e.what());
		return false;
	}
	//catch(...)
	//{
	//	return false;
	//}
	if(mp3->hasIssues())
		WARNING("the \"" << m_pathIn << "\" has issues");

	//ASSERT(m_fields != FieldsMask::None);
	uint mask = static_cast<uint>(m_fields);
	bool bAllFields = (m_fields == FieldsMask::All);
	bool bSeparatorPrintFlag = true;
	// MPEG
	if((mask & FieldsMask::MPEG) || bAllFields)
	{
		if(auto mpeg = mp3->mpegStream())
		{
			printSeparator(bSeparatorPrintFlag);

			auto offset = mp3->mpegStreamOffset();
			auto size = mpeg->getSize();
			LOG("MPEG stream @ offset " <<
				offset << " (0x" << OUT_HEX(offset) << ") +" << size << " (0x" << OUT_HEX(size) << ')');
			
			auto firstFrameOffset = offset + mpeg->getFrameOffset(0);
			LOG("First frame @ offset " << firstFrameOffset << " (0x" << OUT_HEX(firstFrameOffset) << ')');

			LOG(mpeg->getFrameCount() << " frames (" << mpeg->getLength() << " sec)");
			LOG("MPEG " << MPEG::IStream::str(mpeg->getVersion()) << " Layer " << mpeg->getLayer());
			LOG("Bitrate      : " << mpeg->getBitrate() << " kbps" << (mpeg->isVBR() ? " (VBR)" : ""));
			LOG("Sampling Rate: " << mpeg->getSamplingRate() << " Hz");
			LOG("Channel Mode : " << MPEG::IStream::str(mpeg->getChannelMode()));
			LOG("Emphasis     : " << MPEG::IStream::str(mpeg->getEmphasis()));
		}
		else
		{
			printSeparator(bSeparatorPrintFlag);
			WARNING("no MPEG stream");
		}

		mask = mask & ~FieldsMask::MPEG;
	}
	// ID3v1
	if((mask & FieldsMask::ID3v1) || bAllFields)
	{
		if(auto tag = mp3->tagID3v1())
		{
			printSeparator(bSeparatorPrintFlag);

			auto offset = mp3->tagID3v1Offset();
			auto size = tag->getSize();
			LOG("ID3v" << (tag->isV11() ? "1.1" : "1") << " tag @ offset " <<
				offset << " (0x" << OUT_HEX(offset) << ") +" << size << " (0x" << OUT_HEX(size) << ')');

			LOG("Title  : " << tag->getTitle	());
			LOG("Artist : " << tag->getArtist	());
			LOG("Album  : " << tag->getAlbum	());
			LOG("Year   : " << tag->getYear		());
			LOG("Comment: " << tag->getComment	());
			if(tag->isV11())
				LOG("Track:   " << tag->getTrack());

			auto idGenre = tag->getGenreIndex();
			LOG("Genre:   " << idGenre << " (" <<  Tag::genre(idGenre) << ')');
		}
		else if(mask & FieldsMask::ID3v1)
		{
			printSeparator(bSeparatorPrintFlag);
			LOG("No ID3v1 tag");
		}

		mask = mask & ~FieldsMask::ID3v1;
	}
	// ID3v2
	if((mask & FieldsMask::ID3v2) || bAllFields)
	{
		if(auto tag = mp3->tagID3v2())
		{
			printSeparator(bSeparatorPrintFlag);

			auto offset = mp3->tagID3v2Offset();
			auto size = tag->getSize();
			LOG("ID3v2." << tag->getMinorVersion() << '.' << tag->getRevision() << " tag @ offset " <<
				offset << " (0x" << OUT_HEX(offset) << ") +" << size << " (0x" << OUT_HEX(size) << ')');

			#define PRINT_FRAMES(Title, Name, ExFn)	printFrames(Title, *tag, &Tag::IID3v2::get##Name##Count, &Tag::IID3v2::get##Name, ExFn)
			#define PRINT_EX(Title, Name)           PRINT_FRAMES(Title, Name, nullptr)
			#define PRINT(Name)						PRINT_EX(#Name, Name)
			PRINT		(Track);
			PRINT		(Disc);
			PRINT		(BPM);
			PRINT		(Title);
			PRINT		(Artist);
			PRINT		(Album);
			PRINT_EX	("Album Artist", AlbumArtist);
			PRINT		(Year);
			PRINT_FRAMES("Genre", Genre, &Tag::IID3v2::getGenreIndex);
			PRINT		(Comment);
			PRINT		(Composer);
			PRINT		(Publisher);
			PRINT_EX	("Original Artist", OrigArtist);
			PRINT		(Copyright);
			PRINT		(URL);
			PRINT		(Encoded);
			#undef PRINT
			#undef PRINT_EX
			#undef PRINT_FRAMES

			// Print unsupported frames
			auto unknown_frames = tag->getUnknownFrames();
			if(!unknown_frames.empty())
			{
				std::cout << makeAlignedCaption(s_captionWidth, "Unknown frames");
				for_each(unknown_frames.begin(), unknown_frames.end(), [](auto& str)
				{
					std::cout << " " << str;
				});
				std::cout << std::endl;
			}
		}
		else if(mask & FieldsMask::ID3v2)
		{
			printSeparator(bSeparatorPrintFlag);
			LOG("No ID3v1 tag");
		}

		mask = mask & ~FieldsMask::ID3v2;
	}
	// APE
	if((mask & FieldsMask::APE) || bAllFields)
	{
		if(auto tag = mp3->tagAPE())
		{
			printSeparator(bSeparatorPrintFlag);

			auto offset = mp3->tagAPEOffset();
			auto size = tag->getSize();
			LOG("APE tag @ offset " <<
				offset << " (0x" << OUT_HEX(offset) << ") +" << size << " (0x" << OUT_HEX(size) << ')');
		}
		else if(mask & FieldsMask::APE)
		{
			printSeparator(bSeparatorPrintFlag);
			LOG("No APE tag");
		}

		mask = mask & ~FieldsMask::APE;
	}
	// Lyrics
	if((mask & FieldsMask::Lyrics) || bAllFields)
	{
		if(auto tag = mp3->tagLyrics())
		{
			printSeparator(bSeparatorPrintFlag);

			auto offset = mp3->tagLyricsOffset();
			auto size = tag->getSize();
			LOG("Lyrics tag @ offset " <<
				offset << " (0x" << OUT_HEX(offset) << ") +" << size << " (0x" << OUT_HEX(size) << ')');
		}
		else if(mask & FieldsMask::Lyrics)
		{
			printSeparator(bSeparatorPrintFlag);
			LOG("No Lyrics tag");
		}

		mask = mask & ~FieldsMask::Lyrics;
	}

	ASSERT(!mask);

	return true;
}


static void printSeparator(bool& f_ioFirstFlag)
{
	if(f_ioFirstFlag)
		f_ioFirstFlag = false;
	else
		LOG("===");
}


static std::string makeAlignedCaption(uint f_width, const std::string& f_name, int f_index)
{
	auto str = f_name;
	if(f_index >= 0)
		str = str + '(' + std::to_string(f_index) + ')';

	auto n = f_width - str.length();
	if(n > 0)
		str.append(n, ' ');
	str.append(1, ':');

	return str;
}

static void printFrames(const std::string& f_name, const Tag::IID3v2& f_tag, tag_frame_count_getter_t f_pfnCount, tag_frame_getter_t f_pfnGetter, tag_genre_index_getter_t f_pfnGenreIndex)
{
	auto nTags = (f_tag.*f_pfnCount)();
	if(nTags < 2)
	{
		auto caption = makeAlignedCaption(s_captionWidth, f_name);
		if(nTags)
		{
			caption = caption + ' ' + (f_tag.*f_pfnGetter)(0);
			if(f_pfnGenreIndex)
				caption = caption + " (" + std::to_string((f_tag.*f_pfnGenreIndex)(0)) + ')';
		}
		LOG(caption);
	}
	else
	{
		for(unsigned i = 0; i < nTags; ++i)
		{
			auto caption = makeAlignedCaption(s_captionWidth, f_name, i) + ' ' + (f_tag.*f_pfnGetter)(i);
			if(f_pfnGenreIndex)
				caption = caption + " (" + std::to_string((f_tag.*f_pfnGenreIndex)(i)) + ')';
			LOG(caption);
		}
	}
}

// ====================================
bool CmdCutFrames::exec() const
{
	return false;
}

// ====================================
bool CmdHelp::exec() const
{
	static const char* s_name = "mp3_cut";
	LOG( B("NAME") );
	LOG("	" << B(s_name) << " - cut an MP3 file without reencoding");
	LOG("");
	LOG( B("SYNOPSIS") );
	LOG("	" << B(s_name) <<
		" [" << B("-fh") << ']' <<
		" [" << B("-c") << ' ' << U("frame") << ' ' << U("count") << ']' <<
		" [" << B("-C") << ' ' << U("begin") << ' ' << U("end") << ']' <<
		" [" << B("-i") << " [mpeg id3v1 id3v2 ape lyrics]]" <<
		" [" << B("-o") << ' ' << U("file") << ']' <<
		" [" << B("-t") << ' ' << U("count") << ']' <<
		' ' << U("file"));
	LOG("");
	LOG( B("DESCRIPTION") );
	LOG("	The " << B(s_name) << " utility prints information about MPEG data stream, ID3v1, ID3v2, APE and Lyrics tags of an MP3 file, and cuts the MP3 file on a per-frame basis without reencoding.");
	LOG("");
	LOG("The following options are available:");
	LOG("");
	// c
	LOG(B("-c") << ' ' << U("frame") << ' ' << U("count"));
	LOG("	Cut (erase) " << U("count") << " frames starting from the " << U("frame") << ". The " << U("frame") << " is zero-based.");
	LOG("");
	// C
	LOG(B("-C") << ' ' << U("begin") << ' ' << U("end"));
	LOG("	Cut (erase) frames between the " << U("begin") << " second inclusively and the " << U("end") << " second exclusively.");
	LOG("");
	// f
	LOG(B("-f"));
	LOG("	Force processing in case of warnings. With this option an input " << U("file") << " is overwritten if " << U("-o") << " is not specified.");
	LOG("");
	// h
	LOG(B("-h"));
	LOG("	Print help.");
	LOG("");
	// i
	LOG(B("-i") << " [mpeg id3v1 id3v2 ape lyrics]");
	LOG("	Print metadata information.");
	LOG("");
	// o
	LOG(B("-o") << ' ' << U("file"));
	LOG("	Write a result of processing to " << U("file") << ". A corresponding output file is overwritten if " << U("-f") << " is specified.");
	LOG("");
	// t
	LOG(B("-t") << ' ' << U("count"));
	LOG("	Cut " << U("count") << " trailing frames (truncate).");

	// -? ? - trim

	// zero indexed

	return true;
}

