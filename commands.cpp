#include "External/inc/mp3.h"
#include "External/inc/mpeg.h"
#include "External/inc/tag.h"

#include "commands.h"

#include "common.h"



void CmdInfo::printSeparator(bool& f_ioFirstFlag)
{
	if(f_ioFirstFlag)
		f_ioFirstFlag = false;
	else
		LOG("===");
}

bool CmdInfo::exec() const
{
	std::shared_ptr<IMP3> mp3;
	try
	{
LOG(std::endl << ">>> " << m_pathIn);
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

	if(mp3->hasWarnings())
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

			LOG("Track          : " << tag->getTrack		());
			LOG("Disc           : " << tag->getDisc			());
			LOG("BPM            : " << tag->getBPM			());
			LOG("Title          : " << tag->getTitle		());
			LOG("Artist         : " << tag->getArtist		());
			LOG("Album          : " << tag->getAlbum		());
			LOG("Album Artist   : " << tag->getAlbum		());
			LOG("Year           : " << tag->getYear			());
			LOG("Genre          : " << tag->getGenre		() << " (" << tag->getGenreIndex() << ")");
			LOG("Comment        : " << tag->getComment		());
			LOG("Composer       : " << tag->getComposer		());
			LOG("Publisher      : " << tag->getPublisher	());
			LOG("Original Artist: " << tag->getOrigArtist	());
			LOG("Copyright      : " << tag->getCopyright	());
			LOG("URL            : " << tag->getURL			());
			LOG("Encoded        : " << tag->getEncoded		());
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


bool CmdHelp::exec() const
{
	static const char* s_name = "mp3_cut";
	LOG( B("NAME") );
	LOG("	" << B(s_name) << " - cut an MP3 file without reencoding");
	LOG("");
	LOG( B("SYNOPSIS") );
	LOG("	" << B(s_name) << ' ' << B("-[cit]") << ' ' << U("file"));
	LOG("");
	LOG( B("DESCRIPTION") );
	LOG("	The " << B(s_name) << " utility prints information about MPEG data stream, ID3v1, ID3v2, APE and Lyrics tags of an MP3 file, and cuts the MP3 file on a per-frame basis without reencoding.");
	LOG("");
	LOG("The following options are available:");
	LOG("");
	LOG(B("-c") << " [" << U("frame") << "] " << U("count"));
	LOG("	Cut (erases) " << U("count") << " frames of the MPEG stream starting with the " << U("frame") << ". Cut " << U("count") << " leading frames if the " << U("frame") << " is not specified. The " << U("frame") << " is zero-based.");
	LOG("");
	LOG(B("-f"));
	LOG("	Force file overwrite. An input " << U("file") << " is overwritten if " << U("-o") << " is not specified.");
	LOG("");
	LOG(B("-h"));
	LOG("	Print help.");
	LOG("");
	LOG(B("-i") << " [" << U("mpeg") << ' ' << U("id3v1") << ' ' << U("id3v2") << ' ' <<  U("ape") << ' ' << U("lyrics") << "]");
	LOG("	Print metadata information.");
	LOG("");
	LOG(B("-o") << ' ' << U("file"));
	LOG("	Write a result of processing to " << U("file") << ". A corresponding output file is overwritten if " << U("-f") << " is specified.");
	LOG("");
	LOG(B("-t") << ' ' << U("count"));
	LOG("	Cut " << U("count") << " trailing frames (truncate).");

	// -? ? - trim

	// zero indexed

	return true;
}

