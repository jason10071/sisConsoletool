#include "argumentexpression.h"
#include "sedebug/sedebug.h"

#include <sstream>
#include <cstdlib>

using namespace SE;

ArgumentExpression::ArgumentExpression() :
    m_expressionType(ArgumentExpression::EXPRESSION_UNKNOW),
    m_name(""),
    m_value(""),
    m_argumentAttribute(ArgumentExpression::ARGUMENT_UNKNOW),
    m_index(0),
    m_referenceAttribute(ArgumentExpression::REFERENCE_UNKNOW),
    m_description("")
{

}

ArgumentExpression::~ArgumentExpression()
{

}

void
ArgumentExpression::print()
{
    string refOptionStr;
    list<string> refOptionList = getReferenceList(ArgumentExpression::REFERENCE_OPTION);
    for (list<string>::iterator it = refOptionList.begin(); it != refOptionList.end(); it++)
    {
        refOptionStr.append("[");
        refOptionStr.append(*it);
        refOptionStr.append("]");
    }

    string refFileStr;
    list<string> refFileList = getReferenceList(ArgumentExpression::REFERENCE_FILE);
    for (list<string>::iterator it = refFileList.begin(); it != refFileList.end(); it++)
    {
        refFileStr.append("[");
        refFileStr.append(*it);
        refFileStr.append("]");
    }

    string refUnknowStr;
    list<string> refUnknowList = getReferenceList(ArgumentExpression::REFERENCE_UNKNOW);
    for (list<string>::iterator it = refUnknowList.begin(); it != refUnknowList.end(); it++)
    {
        refUnknowStr.append("[");
        refUnknowStr.append(*it);
        refUnknowStr.append("]");
    }

    stringstream log;
    log << "ExpressionType: \"" << getExpressionType() << "\", "
        << "Name: \"" << getName() << "\", "
        << "Value: \"" << getValue() << "\", "
        << "Attribute: \"" << getAttribute() << "\", "
        << "Index: \"" << getIndex() << "\", "
        << "refOptionStr: \"" << refOptionStr << "\", "
        << "refFileStr: \"" << refFileStr << "\", "
        << "refUnknowStr: \"" << refUnknowStr << "\", "
        << "Description: \"" << getDescription() << "\"";

    SEDebug::printLog( log.str() );
}

ArgumentExpression::ExpressionType
ArgumentExpression::getExpressionType()
{
    return this->m_expressionType;
}

void
ArgumentExpression::setExpressionType(ArgumentExpression::ExpressionType expressionType)
{
    this->m_expressionType = expressionType;
}

string
ArgumentExpression::getName()
{
    return this->m_name;
}

void
ArgumentExpression::setName(string name)
{
    this->m_name = name;
}

string
ArgumentExpression::getValue()
{
    return this->m_value;
}

void
ArgumentExpression::setValue(string value)
{
    this->m_value = value;
}

int
ArgumentExpression::getValueToInt()
{
    int v = atoi( this->m_value.c_str() );
    return v;
}

void
ArgumentExpression::setValueFromInt(int value)
{
    stringstream ss;
    ss << value;
    string v = ss.str();
    setValue(v);
}

bool
ArgumentExpression::getValueToBool()
{
    bool v = this->m_value == EXPRESSION_VALUE_TRUE ? true : false;
    return v;
}

void
ArgumentExpression::setValueFromBool(bool value)
{
    string v = value == true ? EXPRESSION_VALUE_TRUE : EXPRESSION_VALUE_FALSE;
    setValue(v);
}

ArgumentExpression::ArgumentAttribute
ArgumentExpression::getAttribute()
{
    return this->m_argumentAttribute;
}

void
ArgumentExpression::setAttribute(ArgumentExpression::ArgumentAttribute attribute)
{
    this->m_argumentAttribute = attribute;
}

int
ArgumentExpression::getIndex()
{
    return this->m_index;
}

void
ArgumentExpression::setIndex(int index)
{
    this->m_index = index;
}

ArgumentExpression::ReferenceAttribute
ArgumentExpression::getReferenceAttribute()
{
    return this->m_referenceAttribute;
}

void
ArgumentExpression::setReferenceAttribute(ArgumentExpression::ReferenceAttribute referenceAttribute)
{
    this->m_referenceAttribute = referenceAttribute;
}

list<string>
ArgumentExpression::getReferenceList(ArgumentExpression::ReferenceAttribute referenceAttribute)
{
    if( referenceAttribute < ArgumentExpression::REFERENCE_UNKNOW )
    {
        return this->m_referenceList[referenceAttribute];
    }

    return this->m_referenceList[ArgumentExpression::REFERENCE_UNKNOW];
}

void
ArgumentExpression::setReferenceList(ArgumentExpression::ReferenceAttribute referenceAttribute, list<string> referenceList)
{
    if( referenceAttribute < ArgumentExpression::REFERENCE_UNKNOW )
    {
        this->m_referenceList[referenceAttribute] = referenceList;
    }
    else
    {
        this->m_referenceList[ArgumentExpression::REFERENCE_UNKNOW] = referenceList;
    }
}

string
ArgumentExpression::getDescription()
{
    return this->m_description;
}

void
ArgumentExpression::setDescription(string description)
{
    this->m_description = description;
}
