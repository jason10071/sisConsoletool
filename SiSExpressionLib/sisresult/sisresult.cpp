#include "sisresult.h"
#include "sedebug/sedebug.h"
#include "sebase/sebase.h"

using namespace SE;

#define EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN          "$"
#define EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT          " : "
#define EXPRESSION_KEYWORD_SECTION_START_BEGIN      "#"
#define EXPRESSION_KEYWORD_SECTION_START_END        "{"
#define EXPRESSION_KEYWORD_SECTION_STOP             "}#"

SiSResult::SiSResult() :
    m_resultExpression(0)
{
    m_resultExpression = new ResultExpression(ResultExpression::TREE_ROOT);
    m_resultExpression->setName("Root");
}

SiSResult::~SiSResult()
{
    delete m_resultExpression;
}

ResultExpression*
SiSResult::getResultExpression()
{
    return this->m_resultExpression;
}

void
SiSResult::parseResult(string resultStr)
{
    clear();

    stringstream log;
    log << "[SiSResult::parseResult(string resultStr)]";
    SEDebug::printLog( log.str() );
//    log.str("");
//    log << "resultStr: \"" << endl << resultStr << endl << "\"";
//    SEDebug::printLog( log.str() );

    /* to lines */
    stringstream resultSS(resultStr);
    string line;
    list<string> lines;
    while( std::getline(resultSS, line) )
    {
        line = SeBase::removeSiSLogBegin(line);

        /* Conform to the format ; [bug] android crash if list is too large */
        if( (line.find( EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN ) != std::string::npos && line.find( EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT ) != std::string::npos) ||
                (line.find( EXPRESSION_KEYWORD_SECTION_START_BEGIN ) != std::string::npos && line.find( EXPRESSION_KEYWORD_SECTION_START_END ) != std::string::npos) ||
                line.find( EXPRESSION_KEYWORD_SECTION_STOP ) != std::string::npos )
        {
            log.str("");
            log << "push_back line: " << line << endl;
            SEDebug::printLog( log.str() );

            eraseEndWhitespaces( line ); // erase '\n'
            lines.push_back( line );
        }
    }

    /* parse */
    parseSectionLines( m_resultExpression, lines);

    /* notify */
    resultChanged();
}

void
SiSResult::parseAttributeLine(ResultExpression* parent, string attributeLine)
{
    /* check format */
    if( !(attributeLine.substr(0, 1) == EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN &&
            attributeLine.find(EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT) != std::string::npos) )
    {
        return;
    }

    /* parse token */
    int beginIdx = attributeLine.find(EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN);
    int splitIdx = attributeLine.find(EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT);
    int endIdx = attributeLine.size();
    string name = attributeLine.substr( beginIdx + 1, splitIdx - beginIdx - 1 );
    string value = attributeLine.substr( splitIdx + 3, endIdx - splitIdx - 3);

    /* insert TREE_LEAF */
    ResultExpression* resultExpression = new ResultExpression(ResultExpression::TREE_LEAF);
    resultExpression->setName(name);
    resultExpression->setValue(value);
    parent->subInsert(resultExpression);
}

void
SiSResult::parseSectionLines(ResultExpression* parent, list<string>& sectionLines)
{
    if( sectionLines.size() == 0 )
    {
        return;
    }

    string line = sectionLines.front();
    sectionLines.pop_front();

    if( line.substr(0, 2) == EXPRESSION_KEYWORD_SECTION_STOP )
    {
        return;
    }

    if( line.substr(0, 1) == EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN &&
            line.find(EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT) != std::string::npos )
    {
        /* attribute */

        stringstream log;
        log << "parse attribute: \"" << line << "\"";
        SEDebug::printLog( log.str() );

        /* parse */
        parseAttributeLine( parent, line );
    }
    else if( line.substr(0, 1) == EXPRESSION_KEYWORD_SECTION_START_BEGIN &&
             endsWith(line, EXPRESSION_KEYWORD_SECTION_START_END) )
    {
        /* section */

        stringstream log;
        log << "parse section: \"" << line << "\"";
        SEDebug::printLog( log.str() );

        /* parse token */
        int sectionBeginIdx = line.find(EXPRESSION_KEYWORD_SECTION_START_BEGIN);
        int sectionEndIdx = line.find(EXPRESSION_KEYWORD_SECTION_START_END);
        string name = line.substr( sectionBeginIdx + 1, sectionEndIdx - sectionBeginIdx - 1 );

        /* insert TREE_NON_LEAF */
        ResultExpression* subResultExpression = new ResultExpression(ResultExpression::TREE_NON_LEAF);
        subResultExpression->setName(name);
        parent->subInsert(subResultExpression);

        /* go sub section lines */
        parseSectionLines( subResultExpression, sectionLines );
    }

    /* go next line */
    parseSectionLines( parent, sectionLines );
}

void
SiSResult::clear()
{
    this->m_resultExpression->clear();

    /* notify */
    resultChanged();
}

string
SiSResult::getPraseErrorInformation()
{
    return this->m_praseErrorSS.str();
}

void
SiSResult::resultChanged()
{
    // do nothing
}

bool
SiSResult::endsWith(const string& str, const string& endStr)
{
    if(endStr.size() > str.size())
    {
        return false;
    }

    return std::equal(str.begin() + str.size() - endStr.size(), str.end(), endStr.begin());
}

void
SiSResult::eraseEndWhitespaces(string& str)
{
    //std::string whitespaces (" \t\f\v\n\r");
    std::string whitespaces ("\t\f\v\n\r"); // remove ' '

    std::size_t found = str.find_last_not_of(whitespaces);
    if ( found != std::string::npos )
    {
        str.erase(found+1);
    }
    else
    {
        str.clear();    // str is all whitespace
    }
}

string
SiSResult::getAttribute(string name, string value)
{
    //cout << EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN << name << EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT << value << endl;

    stringstream text;
    text << EXPRESSION_KEYWORD_ATTRIBUTE_BEGIN << name << EXPRESSION_KEYWORD_ATTRIBUTE_SPLIT << value;
    return text.str();
}

string
SiSResult::getSectionStart(string name)
{
    //cout << EXPRESSION_KEYWORD_SECTION_START_BEGIN << name << EXPRESSION_KEYWORD_SECTION_START_END << endl;

    stringstream text;
    text << EXPRESSION_KEYWORD_SECTION_START_BEGIN << name << EXPRESSION_KEYWORD_SECTION_START_END;
    return text.str();
}

string
SiSResult::getSectionStop()
{
    //cout << EXPRESSION_KEYWORD_SECTION_STOP << endl;

    stringstream text;
    text << EXPRESSION_KEYWORD_SECTION_STOP;
    return text.str();
}
