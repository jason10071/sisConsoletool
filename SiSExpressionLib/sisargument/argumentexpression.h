#ifndef ARGUMENTEXPRESSION_H
#define ARGUMENTEXPRESSION_H

#include <iostream>
#include <list>
#include <string>

#define EXPRESSION_VALUE_TRUE               "true"
#define EXPRESSION_VALUE_FALSE              "false"
#define EXPRESSION_VALUE_DEFAULT_OPTION     "NONE-OPTION"

using namespace std;

namespace SE
{

class ArgumentExpression
{
public:
/*
 * EXPRESSION_1 ========================================================
 * -argument{$SLAVE_INDEX}\n
 * \t#Description{$SLAVE_INDEX}\n
 * \n
 * =====================================================================
 *
 * EXPRESSION_2 ========================================================
 * -argument{$SLAVE_INDEX}=<VALUE|OPTION|DIRECTORY|FILE>\n
 * \t@<OPTION>=[O1|O2|O3]\n
 * \t@<FILE>=[*|*.bin|*.ini|*.xxx]\n
 * \t#Description{$SLAVE_INDEX}\n
 * \n
 * =====================================================================
 *
 * EXPRESSION_3 ========================================================
 * {non-appear-argument}=<VALUE|OPTION|DIRECTORY|FILE>\n
 * \t@<OPTION>=[O1|O2|O3]\n
 * \t@<FILE>=[*|*.bin|*.ini|*.xxx]\n
 * \t#Description{$SLAVE_INDEX}\n
 * \n
 * =====================================================================
*/
    enum ExpressionType
    {
        EXPRESSION_1 = 1,       // Expression-1
        EXPRESSION_2,           // Expression-2
        EXPRESSION_3,           // Expression-3
        EXPRESSION_UNKNOW,
    };

    enum ArgumentAttribute
    {
        ARGUMENT_BOOL = 0,
        ARGUMENT_VALUE,
        ARGUMENT_OPTION,
        ARGUMENT_DIRECTORY,
        ARGUMENT_FILE,
        ARGUMENT_UNKNOW,
    };

    enum ReferenceAttribute
    {
        REFERENCE_OPTION = 0,
        REFERENCE_FILE,
        REFERENCE_UNKNOW,
    };

    ArgumentExpression();
    ~ArgumentExpression();

    void print();

    ArgumentExpression::ExpressionType getExpressionType();
    void setExpressionType(ArgumentExpression::ExpressionType expressionType);

    string getName();
    void setName(string name);

    string getValue();
    void setValue(string value);

    int getValueToInt();
    void setValueFromInt(int value);

    bool getValueToBool();
    void setValueFromBool(bool value);

    ArgumentExpression::ArgumentAttribute getAttribute();
    void setAttribute(ArgumentExpression::ArgumentAttribute attribute);

    int getIndex();
    void setIndex(int index);

    ArgumentExpression::ReferenceAttribute getReferenceAttribute();
    void setReferenceAttribute(ArgumentExpression::ReferenceAttribute referenceAttribute);

    list<string> getReferenceList(ArgumentExpression::ReferenceAttribute referenceAttribute);
    void setReferenceList(ArgumentExpression::ReferenceAttribute referenceAttribute, list<string> referenceList);

    string getDescription();
    void setDescription(string description);

private:
    ExpressionType m_expressionType;
    string m_name;
    string m_value;
    ArgumentExpression::ArgumentAttribute m_argumentAttribute;
    int m_index;
    ArgumentExpression::ReferenceAttribute m_referenceAttribute;
    list<string> m_referenceList[ArgumentExpression::REFERENCE_UNKNOW + 1];
    string m_description;
};

} // SE

#endif // ARGUMENTEXPRESSION_H
