#ifndef EXITSTATUS_H
#define EXITSTATUS_H

#define EXIT_OK             0            /* Test pass or compare result is same */
#define EXIT_ERR           32            /* Error occurs */
#define EXIT_FAIL          33            /* Test fail or compare result is different */
#define EXIT_BADARGU       34            /* Error input argument */
#define EXIT_NODEV         35            /* Device not found */

#define EXIT_ACTIVE_AHEAD          36            /* the active firmware id is AHEAD */
#define EXIT_ACTIVE_BEHEAD         37            /* the active firmware id is BEHEAD */

#define EXIT_UPDATE_FW_WITH_CALIBRATION_OK          40            /*  UPDATE_FW_WITH_CALIBRATION_OK */
#define EXIT_UPDATE_FW_WITH_CALIBRATION_ERR         41            /*  UPDATE_FW_WITH_CALIBRATION_ERR */


#endif
