#include <iostream>

#include "ctexitcode/ctexitcode.h"
#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "updatefwcore/updatefwcore.h"

using namespace std;
using namespace CT;
using namespace CT::CTBP;
using namespace CT::BFP;
using namespace CT::UFWP;
using namespace SiS;
using namespace SiS::Procedure;
using namespace SE;

int main(int argc, char *argv[])
{
    USING_EXIT_CODE;

    UpdateFWParameter* updateFWParameter = new UpdateFWParameter();
    PARSE_ARGS(updateFWParameter, argc, argv);
    DEBUG_SETTING_IF_HAS_ARG_DEBUG_LOG(updateFWParameter);
    SHOW_CTCORE_VERSION;
    SHOW_HELP_IF_HAS_ARG_HELP(updateFWParameter);
    PARSE_ARGUMENT(updateFWParameter);

    UpdateFWCore updateFWCore(updateFWParameter);
    CTCORE_INIT(updateFWCore);
    CTCORE_EXEC(updateFWCore);
    CTCORE_DESTROY(updateFWCore, false);

    delete updateFWParameter;
    RETURN_EXIT_CODE;
}
