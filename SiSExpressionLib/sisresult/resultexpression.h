#ifndef RESULTEXPRESSION_H
#define RESULTEXPRESSION_H

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>

using namespace std;

namespace SE
{

class ResultExpression
{
public:
    enum TreeRole
    {
        TREE_ROOT = 0,
        TREE_NON_LEAF,
        TREE_LEAF,
        TREE_UNDEFINED,
    };

    ResultExpression(ResultExpression::TreeRole treeRole);
    ~ResultExpression();

    ResultExpression::TreeRole getTreeRole();

    /* sub */
    int subSize();
    list<string> subs(TreeRole treeRole = ResultExpression::TREE_UNDEFINED);
    bool subContains(string name);
    bool subContains(int index);
    ResultExpression* subFind(string name);
    ResultExpression* subFind(int index);
    void subInsert(ResultExpression* resultExpression);

    /* attribute */
    string getName();
    void setName(string name);

    string getValue();
    void setValue(string value);

    int getIndex();
    void setIndex(int index);

    int getLevel();
    void setLevel(int level);

    void clear();

private:
    void subErase(string name);

private:
    ResultExpression::TreeRole m_treeRole;

    map<string, ResultExpression*> m_subResults;

    string m_name;
    string m_value;
    int m_index;
    int m_level;
};

} // SE

#endif // RESULTEXPRESSION_H
