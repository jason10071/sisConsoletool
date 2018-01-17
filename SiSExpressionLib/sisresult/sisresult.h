#ifndef SISRESULT_H
#define SISRESULT_H

#include "sisresult/resultexpression.h"

/* [Expression-n (hybrid)]
 * -------------------------------------
$attribute : value\n
...
#section{\n
$attribute : value\n
...
#section{\n
$attribute : value\n
}#\n
...
}#\n
------------------------------------- */

namespace SE
{

class SiSResult
{
public:
    SiSResult();
    ~SiSResult();

    ResultExpression* getResultExpression();

    void parseResult(string resultStr);
    void clear();
    string getPraseErrorInformation();

    virtual void resultChanged();

    static string getAttribute(string name, string value);
    static string getSectionStart(string name);
    static string getSectionStop();

protected:
    void parseAttributeLine(ResultExpression* parent, string attributeLine);
    void parseSectionLines(ResultExpression* parent, list<string>& sectionLines);
    bool endsWith(const string& str, const string& endStr);
    void eraseEndWhitespaces(string& str);

protected:
    ResultExpression* m_resultExpression;
    stringstream m_praseErrorSS;
};

} // SE

#endif // SISRESULT_H
