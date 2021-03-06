/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <test/libsolidity/AnalysisFramework.h>
#include <test/TestCase.h>
#include <liblangutil/Exceptions.h>
#include <libdevcore/AnsiColorized.h>

#include <iosfwd>
#include <string>
#include <vector>
#include <utility>

namespace dev
{
namespace solidity
{
namespace test
{

struct SyntaxTestError
{
	std::string type;
	std::string message;
	std::string sourceName;
	int locationStart;
	int locationEnd;
	bool operator==(SyntaxTestError const& _rhs) const
	{
		return type == _rhs.type &&
			message == _rhs.message &&
			sourceName == _rhs.sourceName &&
			locationStart == _rhs.locationStart &&
			locationEnd == _rhs.locationEnd;
	}
};


class SyntaxTest: public AnalysisFramework, public EVMVersionRestrictedTestCase
{
public:
	static std::unique_ptr<TestCase> create(Config const& _config)
	{
		return std::make_unique<SyntaxTest>(_config.filename, _config.evmVersion, false);
	}
	static std::unique_ptr<TestCase> createErrorRecovery(Config const& _config)
	{
		return std::make_unique<SyntaxTest>(_config.filename, _config.evmVersion, true);
	}
	SyntaxTest(std::string const& _filename, langutil::EVMVersion _evmVersion, bool _parserErrorRecovery = false);

	TestResult run(std::ostream& _stream, std::string const& _linePrefix = "", bool _formatted = false) override;

	void printSource(std::ostream &_stream, std::string const &_linePrefix = "", bool _formatted = false) const override;
	void printUpdatedExpectations(std::ostream& _stream, std::string const& _linePrefix) const override
	{
		if (!m_errorList.empty())
			printErrorList(_stream, m_errorList, _linePrefix, false);
	}

	static std::string errorMessage(Exception const& _e);
protected:
	void setupCompiler();
	void parseAndAnalyze();
	void filterObtainedErrors();

	static void printErrorList(
		std::ostream& _stream,
		std::vector<SyntaxTestError> const& _errors,
		std::string const& _linePrefix,
		bool _formatted = false
	);

	virtual bool printExpectationAndError(std::ostream& _stream, std::string const& _linePrefix = "", bool _formatted = false);

	static std::vector<SyntaxTestError> parseExpectations(std::istream& _stream);

	std::map<std::string, std::string> m_sources;
	std::vector<SyntaxTestError> m_expectations;
	std::vector<SyntaxTestError> m_errorList;
	bool m_optimiseYul = true;
	langutil::EVMVersion const m_evmVersion;
	bool m_parserErrorRecovery = false;
};

}
}
}
