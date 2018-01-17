#include "resultexpression.h"
#include "sedebug/sedebug.h"

using namespace SE;

ResultExpression::ResultExpression(TreeRole treeRole) :
    m_treeRole(treeRole),
    m_name(""),
    m_level(0)
{

}

ResultExpression::~ResultExpression()
{
    clear();

    stringstream log;
    log << "ResultExpression::~ResultExpression() done, \"" << getName() << "\" (" << getTreeRole() << ")";
    SEDebug::printLog( log.str() );
}

ResultExpression::TreeRole
ResultExpression::getTreeRole()
{
    return this->m_treeRole;
}

int
ResultExpression::subSize()
{
    return this->m_subResults.size();
}

list<string>
ResultExpression::subs(ResultExpression::TreeRole treeRole)
{
    list<string> subList;

    for (int idx = 0; idx < subSize(); idx++)
    {
        if( subContains(idx) )
        {
            ResultExpression* resultExpression = subFind(idx);

            if( treeRole == ResultExpression::TREE_UNDEFINED ||
                    treeRole == resultExpression->getTreeRole() )
            {
                subList.push_back( resultExpression->getName() );
            }
        }
    }

    return subList;
}

bool
ResultExpression::subContains(string name)
{
    if( m_subResults.find( name ) != m_subResults.end() )
    {
        return true;
    }

    return false;
}

bool
ResultExpression::subContains(int index)
{
    if( subFind(index) != 0 )
    {
        return true;
    }

    return false;
}

ResultExpression*
ResultExpression::subFind(string name)
{
    ResultExpression* resultExpression = 0;

    map<string, ResultExpression*>::const_iterator it = m_subResults.find( name );
    if( it != m_subResults.end() )
    {
        resultExpression = it->second;
    }

    return resultExpression;
}

ResultExpression*
ResultExpression::subFind(int index)
{
    ResultExpression* resultExpression = 0;

    for(map<string, ResultExpression*>::iterator it = m_subResults.begin(); it != m_subResults.end(); ++it)
    {
        if( it->second->getIndex() == index )
        {
            resultExpression = it->second;
            break;
        }
    }

    return resultExpression;
}

void
ResultExpression::subInsert(ResultExpression* resultExpression)
{
    string name = resultExpression->getName();

    /* index */
    if( subContains( name ) )
    {
        resultExpression->setIndex( subFind( name )->getIndex() ); // use the same index
        subErase( name );
    }
    else
    {
        resultExpression->setIndex( subSize() ); // index + 1
    }

    /* level */
    resultExpression->setLevel( m_level + 1 );

    m_subResults.insert( map<string, ResultExpression*>::value_type( name, resultExpression) );
}

void
ResultExpression::subErase(string name)
{
    if( subContains( name ) )
    {
        ResultExpression* resultExpression = subFind( name );
        m_subResults.erase( name );
        delete resultExpression;
    }
}

void
ResultExpression::clear()
{
    for(map<string, ResultExpression*>::iterator it = m_subResults.begin(); it != m_subResults.end(); ++it)
    {
        ResultExpression* resultExpression = it->second;
        delete resultExpression;
    }

    m_subResults.clear();
}

string
ResultExpression::getName()
{
    return this->m_name;
}

void
ResultExpression::setName(string name)
{
    this->m_name = name;
}

string
ResultExpression::getValue()
{
    return this->m_value;
}

void
ResultExpression::setValue(string value)
{
    this->m_value = value;
}

int
ResultExpression::getIndex()
{
    return this->m_index;
}

void
ResultExpression::setIndex(int index)
{
    this->m_index = index;
}

int
ResultExpression::getLevel()
{
    return this->m_level;
}

void
ResultExpression::setLevel(int level)
{
    this->m_level = level;
}
