#include "ApplicationParameter.h"
#include "UpdateFWParameter.h"
#include "GetFirmwareIdParameter.h"
#include "OKFWParameter.h"
#include "CalibrationParameter.h"
#include "CalibrationWithOpenShortParameter.h"
#include "OpenShortParameter.h"
#include "DumpROMParameter.h"
#include "SoftResetParameter.h"
#include "GetFwModeParameter.h"
#include "CheckITOParameter.h"

/*===========================================================================*/
ApplicationParameter * ApplicationParameter::create(ApplicationType type)
{
    ApplicationParameter *p = 0;

    switch (type) {
        case UPDATE_FW:
            p = new UpdateFWParameter();
            break;

        case GET_FW_ID:
            p = new GetFirmwareIdParameter();
            break;

        case OK_FW:
            p = new OKFWParameter();
            break;

        case CALIBRATION:
            p = new CalibrationParameter();
            break;

        case CALIBRATION_WITH_OPENSHORT:
            p = new CalibrationWithOpenShortParameter();
            break;

        case OPEN_SHORT:
            p = new OpenShortParameter();
            break;

        case DUMP_ROM:
            p = new DumpROMParameter();
            break;

        case SOFT_RESET:
            p = new SoftResetParameter();
            break;

        case GET_FW_MODE:
        	p = new GetFwModeParameter();
        	break;

        case CHECK_ITO:
        	p = new CheckITOParameter();
        	break;

        default:
            p = 0;
            break;
    }

    return p;
}
/*===========================================================================*/
