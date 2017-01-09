#pragma once


#include <memory>
#include <string>


class Command
{
public:
	explicit Command() = default;
	virtual ~Command() {}

	void suppressWarnings() { m_force = true; }

	virtual bool exec() const = 0;

protected:
	bool m_force;
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
		m_pathIn(f_pathIn),
		m_fields(f_fields)
	{}

	bool exec() const final override;

private:
	std::string	m_pathIn;
	FieldsMask	m_fields;
};


class CmdCutFrames final : public Command
{
public:
	CmdCutFrames(const std::string& f_pathIn, const std::string& f_pathOut,
				 unsigned f_frame, unsigned f_count):
		m_pathIn(f_pathIn),
		m_pathOut(f_pathOut),
		m_frame(f_frame),
		m_count(f_count)
	{}

	bool exec() const final override;

private:
	std::string	m_pathIn;
	std::string	m_pathOut;

	unsigned	m_frame;
	unsigned	m_count;
};

