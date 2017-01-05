#pragma once


#include <memory>
#include <string>


class Command
{
public:
	explicit Command() = default;
	Command(const std::string& f_pathIn): m_pathIn(f_pathIn) {}
	virtual ~Command() {}

	void allowOverwrite() { m_overwrite = true; }

	virtual bool exec() const = 0;

protected:
	std::string m_pathIn;
	std::string m_pathOut;
	bool m_overwrite;
};


class CmdHelp final : public Command
{
public:
	explicit CmdHelp() = default;

	bool exec() const final override;
};


class CmdInfo final : public Command
{
public:
	enum FieldsMask
	{
		All		= 0,
		MPEG	= 1 << 0,
		ID3v1	= 1 << 1,
		ID3v2	= 1 << 2,
		APE		= 1 << 3,
		Lyrics	= 1 << 4
	};

public:
	CmdInfo(const std::string& f_pathIn, FieldsMask f_fields):
		Command(f_pathIn),
		m_fields(f_fields)
	{}

	bool exec() const final override;

private:
	FieldsMask m_fields;
};


class CmdCutFrames final : public Command
{
public:
	CmdCutFrames(const std::string& f_pathIn, unsigned f_frame, unsigned f_count):
		Command(f_pathIn),
		m_frame(f_frame),
		m_count(f_count)
	{}

	bool exec() const final override;

private:
	unsigned m_frame;
	unsigned m_count;
};

