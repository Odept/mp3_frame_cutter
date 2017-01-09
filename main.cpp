#include "commands.h"

#include "common.h"


template<typename T>
static T orEnums(T f_e0, T f_e1)
{
	return static_cast<T>(static_cast<int>(f_e0) | f_e1);
}


static std::unique_ptr<Command> parseInfoArgs(const std::string& f_pathIn,
											  const char* f_args[], uint f_nArgs,
											  uint& f_ioCurArg)
{
	if(f_pathIn.empty())
	{
		ERROR("no input file specified");
		return nullptr;
	}

	auto mask = CmdInfo::FieldsMask::All;

	for(++f_ioCurArg; f_ioCurArg < f_nArgs; ++f_ioCurArg)
	{
		std::string opt(f_args[f_ioCurArg]);

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


static std::unique_ptr<Command> parseCutFramesArgs(const std::string& f_pathIn, const std::string& f_pathOut,
												   const char* f_args[], uint f_nArgs,
												   uint& f_ioCurArg)
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
		return std::make_unique<CmdCutFrames>(f_pathIn, f_pathOut, frame, count);
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


static bool parseOutArgs(const char* f_args[], uint f_nArgs, std::string& f_outPathOut)
{
	std::string pathOut;

	for(uint i = 0; i < f_nArgs; ++i)
	{
		std::string cmd(f_args[i]);
		if(cmd != "-o")
			continue;

		if(!pathOut.empty())
		{
			ERROR("multiple \"-o\" options");
			return false;
		}

		if(++i >= f_nArgs)
		{
			ERROR("no output file path is specified");
			return false;
		}
		auto arg = f_args[i];

		if(arg[0] == '-')
		{
			ERROR("output file path is expected");
			return false;
		}
		pathOut = arg;
	}

	f_outPathOut = pathOut;
	return true;
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

	std::string fileOut;
	if( !parseOutArgs(f_args, nArgs, fileOut) )
		return nullptr;

	std::unique_ptr<Command> sp;
	bool bForce = false;

	for(uint i = 0; i < nArgs;)
	{
		std::string cmd(f_args[i]);

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
		else if(cmd == "-o")
		{
			// "-o" is pre-parsed in the beginning of the function
			ASSERT(!fileOut.empty());
			i += 2;
			continue;
		}
		else if(cmd == "-c")
		{
			if(sp)
				return invalidOp(cmd);
			sp = parseCutFramesArgs(fileIn, fileOut, f_args, nArgs, i);
			if(!sp)
				return nullptr;
			continue;
		}
		else if(cmd == "-i")
		{
			if(sp)
				return invalidOp(cmd);
			sp = parseInfoArgs(fileIn, f_args, nArgs, i);
			if(!sp)
				return nullptr;
			continue;
		}

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
	if( !cmd->exec() )
		return 1;

	return 0;
}

