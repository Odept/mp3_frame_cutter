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
	bool bOverwrite = false;

	for(uint i = 0; i < nArgs;)
	{
		std::string cmd(f_args[i], strnlen(f_args[i], 3));

		if(cmd == "-h")
		{
			if(sp)
				return invalidOp(cmd);
			return std::make_unique<CmdHelp>();
		}
		else if(cmd == "-f")
		{
			bOverwrite = true;
			++i;
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

	if(bOverwrite)
		sp->allowOverwrite();

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

