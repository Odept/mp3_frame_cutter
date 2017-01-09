#include "commands.h"

#include "common.h"


template<typename T>
static T orEnums(T f_e0, T f_e1)
{
	return static_cast<T>(static_cast<int>(f_e0) | f_e1);
}


static std::unique_ptr<Command> parseInfoArgs(const std::string& f_pathIn, uint f_nArgs, const char* f_args[], uint& f_ioCurArg)
{
	if(f_pathIn.empty())
	{
		ERROR("no input file specified");
		return nullptr;
	}

	auto mask = CmdInfo::FieldsMask::All;

	for(++f_ioCurArg; f_ioCurArg < f_nArgs; ++f_ioCurArg)
	{
		std::string opt(f_args[f_ioCurArg], strnlen(f_args[f_ioCurArg], 16));

		if(opt == "mpeg")
			mask = orEnums(mask, CmdInfo::FieldsMask::MPEG);
		else if(opt == "id3v1")
			mask = orEnums(mask, CmdInfo::FieldsMask::ID3v1);
		else if(opt == "id3v2")
			mask = orEnums(mask, CmdInfo::FieldsMask::ID3v2);
		else if(opt == "ape")
			mask = orEnums(mask, CmdInfo::FieldsMask::APE);
		else if(opt == "lyrics")
			mask = orEnums(mask, CmdInfo::FieldsMask::Lyrics);
		else
			break;
	}

	return std::make_unique<CmdInfo>(f_pathIn, mask);
}


static std::unique_ptr<Command> parseCutFramesArgs(const std::string& f_pathIn, uint f_nArgs, const char* f_args[], uint& f_ioCurArg)
{
	if(f_pathIn.empty())
	{
		ERROR("no input file specified");
		return nullptr;
	}

	try
	{
		uint frame, count;
		size_t errIndex;

		// Start frame
		if(++f_ioCurArg >= f_nArgs)
		{
			ERROR("no frame to start cutting from is specified");
			return nullptr;
		}
		auto iFrame = std::stol(f_args[f_ioCurArg], &errIndex, 0);
		if(iFrame < 0)
			throw std::out_of_range("the start frame number can't be negative");
		if(char c = f_args[f_ioCurArg][errIndex])
			throw std::invalid_argument(std::string("unexpected character '") + std::string(1, c) + "'");
		frame = iFrame;

		// Frames count
		if(++f_ioCurArg >= f_nArgs)
		{
			ERROR("no number of frames to cut is specified");
			return nullptr;
		}
		auto iCount = std::stol(f_args[f_ioCurArg], &errIndex, 0);
		if(iCount <= 0)
			throw std::out_of_range("a number of frames to cut must be greater than zero");
		if(char c = f_args[f_ioCurArg][errIndex])
			throw std::invalid_argument(std::string("unexpected character '") + std::string(1, c) + "'");
		count = iCount;

		++f_ioCurArg;
		return std::make_unique<CmdCutFrames>(f_pathIn, frame, count);
	}
	catch(const std::invalid_argument& e)
	{
		ERROR("the value \"" << f_args[f_ioCurArg] << "\" is invalid (" << e.what() << ')');
	}
	catch(const std::out_of_range& e)
	{
		ERROR("the value \"" << f_args[f_ioCurArg] << "\" is out of bounds (" << e.what() << ')');
	}

	return nullptr;
}


static std::unique_ptr<const Command> invalidOp(const std::string& f_op)
{
	ERROR("unexpected option \"" << f_op << '"');
	return nullptr;
}


static std::unique_ptr<const Command> parseArgs(uint f_nArgs, const char* f_args[])
{
	if(!f_nArgs)
		return std::make_unique<CmdHelp>();

	uint nArgs = f_nArgs;
	std::string fileIn;
	if(f_args[nArgs - 1][0] != '-')
	{
		--nArgs;
		fileIn = f_args[nArgs];
	}

	std::unique_ptr<Command> sp;
	bool bForce = false;

	for(uint i = 0; i < nArgs;)
	{
		std::string cmd(f_args[i], strnlen(f_args[i], 3));

		if(cmd == "-f")
		{
			bForce = true;
			++i;
			continue;
		}
		else if(cmd == "-h")
		{
			if(sp)
				return invalidOp(cmd);
			return std::make_unique<CmdHelp>();
		}
		else if(cmd == "-c")
		{
			if(sp)
				return invalidOp(cmd);
			sp = parseCutFramesArgs(fileIn, nArgs, f_args, i);
			if(!sp)
				return nullptr;
			continue;
		}
		else if(cmd == "-i")
		{
			if(sp)
				return invalidOp(cmd);
			sp = parseInfoArgs(fileIn, nArgs, f_args, i);
			if(!sp)
				return nullptr;
			continue;
		}
		//, return std::make_unique<CmdInfo>();
		//else if(cmd == "-t")
		//	return std::make_unique<CmdInfo>();
		//else if(cmd == "-c")
		//	return std::make_unique<CmdInfo>();

		return invalidOp(cmd);
	}

	if(!sp)
	{
		ERROR("no command specified");
		return nullptr;
	}

	if(bForce)
		sp->suppressWarnings();

	return std::move(sp);
}

// ============================================================================
int main(int argc, const char* args[])
{
	auto cmd = parseArgs(argc - 1, &args[1]);
	if(!cmd)
		return 2;
	if( cmd->exec() )
		return 1;

	return 0;
}

