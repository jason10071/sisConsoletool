#include "OpenShortResult.h"
#include "OpenShortData.h"
#include <stdlib.h>
#include <string.h>

OpenShortResult::OpenShortResult():
    mWidth(0)
    ,mHeight(0)
    ,mPointReulst(0)
    ,mLineResult(0)
    ,mPointNG(0)
	,mSecondNG(0)
    ,mLineNG(0)
    ,mCycleTxDiff(0)
    ,mTXPointShortResult(0)
    ,mCycleRxDiff(0)
    ,mRXPointShortResult(0)
    ,mLineNearFarValue(0)
    ,mLineNearFarResult(0)
    ,lostSide(0)
{

    mPointNG = 0;
    mLineNG = 0;
    mPointReulst = 0;
    mLineResult = 0;

    setWH(DEFAULT_W, DEFAULT_H);
}

OpenShortResult::OpenShortResult(int w, int h) :
    mWidth(w)
    ,mHeight(h)
    ,mPointReulst(0)
    ,mLineResult(0)
    ,mPointNG(0)
	,mSecondNG(0)
    ,mLineNG(0)
    ,mCycleTxDiff(0)
    ,mTXPointShortResult(0)
    ,mCycleRxDiff(0)
    ,mRXPointShortResult(0)
    ,mLineNearFarValue(0)
    ,mLineNearFarResult(0)
    ,lostSide(0)
{

    mPointNG = 0;
    mLineNG = 0;
    mPointReulst = 0;
    mLineResult = 0;

    setWH(w, h);
}

OpenShortResult::~OpenShortResult()
{
    delete [] mPointReulst;
    delete [] mLineResult;

    delete [] mCycleTxDiff; // MutualShortExtraCheck
    delete [] mTXPointShortResult; // MutualShortExtraCheck
    delete [] mCycleRxDiff; // MutualShortExtraCheck
    delete [] mRXPointShortResult; // MutualShortExtraCheck

    delete [] mLineNearFarValue; // MutualNearFarCheck
    delete [] mLineNearFarResult; // MutualNearFarCheck
}

void
OpenShortResult::setWH(int w, int h)
{

    mPointNG = 0;
    mLineNG = 0;


    if(mLineResult != 0)
    {

        delete [] mPointReulst;
        delete [] mLineResult;

        delete [] mCycleTxDiff; // MutualShortExtraCheck
        delete [] mTXPointShortResult; // MutualShortExtraCheck
        delete [] mCycleRxDiff; // MutualShortExtraCheck
        delete [] mRXPointShortResult; // MutualShortExtraCheck

        delete [] mLineNearFarValue; // MutualNearFarCheck
        delete [] mLineNearFarResult; // MutualNearFarCheck

        mPointReulst = 0;
        mLineResult = 0;

        mCycleTxDiff = 0; // MutualShortExtraCheck
        mTXPointShortResult = 0; // MutualShortExtraCheck
        mCycleRxDiff = 0; // MutualShortExtraCheck
        mRXPointShortResult = 0; // MutualShortExtraCheck

        mLineNearFarValue = 0; // MutualNearFarCheck
        mLineNearFarResult = 0; // MutualNearFarCheck
    }


    mWidth = w;
    mHeight = h;

    mPointReulst = new unsigned int[mWidth * mHeight];
    mLineResult = new unsigned int[mWidth + mHeight];

    memset(mPointReulst, 0, sizeof(unsigned int) * mWidth * mHeight);
    memset(mLineResult, 0, sizeof(unsigned int) * (mWidth + mHeight));

    // MutualShortExtraCheck, begin
    mCycleTxDiff = new float[mWidth * mHeight];
    mTXPointShortResult = new unsigned int[mWidth * mHeight];
    mCycleRxDiff = new float[mWidth * mHeight];
    mRXPointShortResult = new unsigned int[mWidth * mHeight];
    memset(mCycleTxDiff, 0, sizeof(float) * mWidth * mHeight);
    memset(mTXPointShortResult, POINT_PASS, sizeof(unsigned int) * mWidth * mHeight);
    memset(mCycleRxDiff, 0, sizeof(float) * mWidth * mHeight);
    memset(mRXPointShortResult, POINT_PASS, sizeof(unsigned int) * mWidth * mHeight);
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    mLineNearFarValue = new float[mWidth + mHeight];
    memset(mLineNearFarValue, 0, sizeof(float) * (mWidth + mHeight));
    mLineNearFarResult = new unsigned int[mWidth + mHeight];
    memset(mLineNearFarResult, 0, sizeof(unsigned int) * (mWidth + mHeight));
    // MutualNearFarCheck, end
}

// MutualShortExtraCheck, begin
void
OpenShortResult::setCycleTxDiff(int x, int y, float result)
{
    mCycleTxDiff[x * mHeight + y] = result;
}

void
OpenShortResult::setTXPointShortResult(int x, int y, unsigned int result)
{
    mTXPointShortResult[x * mHeight + y] = result;
}

void
OpenShortResult::setCycleRxDiff(int x, int y, float result)
{
    mCycleRxDiff[x * mHeight + y] = result;
}

void
OpenShortResult::setRXPointShortResult(int x, int y, unsigned int result)
{
    mRXPointShortResult[x * mHeight + y] = result;
}

float
OpenShortResult::getCycleTxDiff(int x, int y)
{
    return mCycleTxDiff[x * mHeight + y];
}

unsigned int
OpenShortResult::getTXPointShortResult(int x, int y)
{
    return mTXPointShortResult[x * mHeight + y];
}

float
OpenShortResult::getCycleRxDiff(int x, int y)
{
    return mCycleRxDiff[x * mHeight + y];
}

unsigned int
OpenShortResult::getRXPointShortResult(int x, int y)
{
    return mRXPointShortResult[x * mHeight + y];
}
// MutualShortExtraCheck, end

// MutualNearFarCheck, begin
void
OpenShortResult::setXLineNearFarValue(int x, float result)
{
    mLineNearFarValue[x] = result;
}

void
OpenShortResult::setYLineNearFarValue(int y, float result)
{
    mLineNearFarValue[mWidth + y] = result;
}

float
OpenShortResult::getXLineNearFarValue(int x)
{
    return mLineNearFarValue[x];
}

float
OpenShortResult::getYLineNearFarValue(int y)
{
    return mLineNearFarValue[mWidth + y];
}

void
OpenShortResult::setXLineNearFarResult(int x, unsigned int result)
{
    mLineNearFarResult[x] = result;
}

void
OpenShortResult::setYLineNearFarResult(int y, unsigned int result)
{
    mLineNearFarResult[mWidth + y] = result;
}

unsigned int
OpenShortResult::getXLineNearFarResult(int x)
{
    return mLineNearFarResult[x];
}

unsigned int
OpenShortResult::getYLineNearFarResult(int y)
{
    return mLineNearFarResult[mWidth + y];
}
// MutualNearFarCheck, end

void
OpenShortResult::setPointResult(int x, int y, unsigned int result)
{

    mPointReulst[x * mHeight + y] = result;
}

void
OpenShortResult::setXLineResult(int x, unsigned int result)
{
    mLineResult[x] = result;
}

void
OpenShortResult::setYLineResult(int y, unsigned int result)
{
    mLineResult[mWidth + y] = result;
}


void
OpenShortResult::setSecondNG(int secondNgNum)
{
	mSecondNG = secondNgNum;
}

void
OpenShortResult::setPointNG(int ngNum)
{
    mPointNG = ngNum;
}

void
OpenShortResult::setLineNG(int ngNum)
{
    mLineNG = ngNum;
}



void
OpenShortResult::appendPointResult(int x, int y, unsigned int result)
{
    mPointReulst[x * mHeight + y] |= result;
}

void
OpenShortResult::appendXLineResult(int x, unsigned int result)
{
    mLineResult[x] |= result;
}

void
OpenShortResult::appendYLineResult(int y, unsigned int result)
{
    mLineResult[mWidth + y] |= result;
}


int
OpenShortResult::getSecondNG()
{
    return mSecondNG;
}


int
OpenShortResult::getPointNG()
{
    return mPointNG;
}

int
OpenShortResult::getLineNG()
{
    return mLineNG;
}

int
OpenShortResult::getWidth()
{
    return mWidth;
}

int
OpenShortResult::getHeight()
{
    return mHeight;
}

unsigned int
OpenShortResult::getPointResult(int x, int y)
{
    return mPointReulst[x * mHeight + y];
}


unsigned int
OpenShortResult::getXLineResult(int x)
{
    return mLineResult[x];
}

unsigned int
OpenShortResult::getYLineResult(int y)
{
    return mLineResult[mWidth + y];
}

void
OpenShortResult::dumpResult()
{

    if(mLineNG != 0)
    {
        for(int i=0; i<mWidth; i++)
        {
            unsigned int lineResult = getXLineResult(i);
            if(getXLineResult(i) != LINE_PASS)
            {
                printf("line x %d is suspicious, state is %08x.\n", i+1, lineResult);
            }
        }

        for(int j=0;j<mHeight;j++)
        {
            unsigned int lineResult = getYLineResult(j);
            if(getYLineResult(j) != LINE_PASS)
            {
                printf("line y %d is suspicious, state is %08x.\n", j+1, lineResult);
            }                
        }    
    }

    if(mPointNG !=0)
    {
        for(int i=0; i<mWidth; i++)
        {
            for(int j=0;j<mHeight;j++)
            {
                unsigned int pointResult = getPointResult(i, j);
                if(pointResult != POINT_PASS)
                {
                    printf("point %d, %d is suspicious, state is %08x.\n", i+1, j+1, pointResult);
                }                                  
            }    
        }
    }
}


int
OpenShortResult::transferPointResultToWindowsResult(unsigned int result)
{
    if( ((result & POINT_OVEROPEN_VOLTAGE_ON_TARGET) != 0) )
    {
        return 4;
    }
    else if( ((result & POINT_OPEN_VOLTAGE_ON_TARGET) != 0) )
    {
        return 0;
    }
    else if( ((result & POINT_OVEROPEN_VOLTAGE_ON_SAMPLE) != 0) || ((result & POINT_OPEN_VOLTAGE_ON_SAMPLE) != 0) ||
            ((result & POINT_OVEROPEN_TIME_ON_SAMPLE) != 0) || ((result & POINT_OPEN_TIME_ON_SAMPLE) != 0) )
    {
        return 7;
    }
    else
    {
        return 1;
    }
}

void
OpenShortResult::transferPointResultToErrorReasonSimple(unsigned int result, char* buf)
{
	bool first = true;
	
	strcpy(buf, "");    

	if( ((result & POINT_OVEROPEN_VOLTAGE_ON_TARGET) != 0) || ((result & POINT_OPEN_VOLTAGE_ON_TARGET) != 0) )
    {
        strcat(buf, "A");
		first = false;
    }

	if( ((result & POINT_OVEROPEN_VOLTAGE_ON_SAMPLE) != 0) || ((result & POINT_OPEN_VOLTAGE_ON_SAMPLE) != 0) )
    {
        if(!first)
        {     
            strcat(buf, "/");   
        }
        strcat(buf, "B");
		first = false;
    }

    if( ((result & POINT_OVEROPEN_TIME_ON_SAMPLE) != 0) || ((result & POINT_OPEN_TIME_ON_SAMPLE) != 0) )
    {
        if(!first)
        {     
            strcat(buf, "/");   
        }
        strcat(buf, "C");
		first = false;
    }
    
}


void
OpenShortResult::transferPointResultToErrorReason(unsigned int result, char* buf)
{

    bool first = true;

    strcpy(buf, "");

    if( ((result & POINT_OVEROPEN_VOLTAGE_ON_SAMPLE) != 0) || ((result & POINT_OPEN_VOLTAGE_ON_SAMPLE) != 0) )
    {
        if(!first)
        {     
            strcat(buf, " ");   
        }
        strcat(buf, "Voltage vs Sample");
        first = false;
    }

    if( ((result & POINT_OVEROPEN_TIME_ON_SAMPLE) != 0) || ((result & POINT_OPEN_TIME_ON_SAMPLE) != 0) )
    {
        if(!first)
        {     
            strcat(buf, " ");   
        }
        strcat(buf, "Calibration vs Sample");
        first = false;

    }

    if( ((result & POINT_OVEROPEN_VOLTAGE_ON_TARGET) != 0) || ((result & POINT_OPEN_VOLTAGE_ON_TARGET) != 0) )
    {
        if(!first)
        {     
            strcat(buf, " ");   
        }
        strcat(buf, "Voltage vs Target");
        first = false;
    }
}

void
OpenShortResult::fprintData(FILE *fp2D, OpenShortData* data, OpenShortResult *result, OpenShortConfig *config, int tableID)
{
    char buff[255];

	int width = data->getWidth();
	int height = data->getHeight();    

    fprintf(fp2D, "Y\\X,");
    for ( int i = 0; i < width; i++) {

        if(config->orderConfig.InvXorder == 1) {
            sprintf(buff, "%c%d", 'X', width-i);
            fprintf(fp2D, "%13s,", buff);
        }
        else {
            sprintf(buff, "%c%d", 'X', i+1);
            fprintf(fp2D, "%13s,", buff);
        }
    }

    fprintf(fp2D, "\r\n");

    int idxI, idxJ;

    for ( int j = 0; j < height; j ++)
    {
        if(config->orderConfig.InvYorder == 1) {
            sprintf(buff, "%c%d", 'Y', j+1);
            fprintf(fp2D, "%3s,", buff);
            idxJ = j;
        }
        else {
            sprintf(buff, "%c%d", 'Y', height-j);
            fprintf(fp2D, "%3s,", buff);
            idxJ = height - j - 1;
        }
        for ( int i = 0; i < width; i ++)
        {
            if (config->orderConfig.InvXorder== 1)
            {
                idxI = width - i - 1;
            } 
            else
            {
                idxI = i;
            }

			int testResult;
            char failPara[10];

            switch (tableID) {                                
                case ResultID:
					testResult = OpenShortResult::transferPointResultToWindowsResult(result->getPointResult(idxI, idxJ));
					OpenShortResult::transferPointResultToErrorReasonSimple(result->getPointResult(idxI, idxJ), failPara);
                    if(testResult == 1)
                        sprintf(buff, "%d", 1);
                    else {
                        sprintf(buff, "%d(%s)", testResult, failPara);
                    }
                    break;
                case VoltageID:
					if(data->getReadData())
	                    sprintf(buff, "%.3f", data->getBaseVoltage(idxI, idxJ));
					else
						sprintf(buff, "%.3f", data->getRawVoltage(idxI, idxJ));
                    break;
                case LOOPID:
                    sprintf(buff, "%.0f", data->getLoop(idxI, idxJ));
                    break;
                case CYCLEID:
                    sprintf(buff, "%.3f", data->getCycle(idxI, idxJ));
                    break;

                // MutualShortExtraCheck, begin
                case CycleTxDiffID:
                    sprintf(buff, "%.3f", result->getCycleTxDiff(idxI, idxJ));
                    break;
                case CycleRxDiffID:
                    sprintf(buff, "%.3f", result->getCycleRxDiff(idxI, idxJ));
                    break;
                case TXPointShortResultID:
                    if(result->getTXPointShortResult(idxI, idxJ) == POINT_PASS)
                    {
                        sprintf(buff, "%d", 1);
                    } else 
                    {
                        sprintf(buff, "%d", 0);
                    }
                    break;
                case RXPointShortResultID:
                    if(result->getRXPointShortResult(idxI, idxJ) == POINT_PASS)
                    {
                        sprintf(buff, "%d", 1);
                    } else 
                    {
                        sprintf(buff, "%d", 0);
                    }
                    break;
                // MutualShortExtraCheck, end

                default:
                    break;

            }
            fprintf(fp2D, "%13s,", buff);
        }      
        fprintf(fp2D, "\r\n");
    }
}

bool
OpenShortResult::saveOpenShortResult(FILE *file, OpenShortData* data, OpenShortResult *result, OpenShortConfig *config)
{
    if(!file)
    {
        printf("null file descriptor\n");
        return false;
    }

    int height = data->getHeight();
    int width = data->getWidth();
    int btnChannel = data->getBtnChannel();
    // char reasonBuf[255] = {0};

	fprintf(file, "NG Num,%6d, Result Comment: , 1 = Pass (Green --> Under +/- Threshold of Target or Compared with Golden Sample)\r\n", result->getPointNG() + result->getLineNG());
    fprintf(file, "Node NG,%5d,                 , 0 = Fail (Purple --> Less than Target Voltage)\r\n", result->getPointNG());
    fprintf(file, "Line NG,%5d,                 , 4 = Fail (Red --> More than Target Voltage)\r\n", result->getLineNG());
    fprintf(file, "                             ,,, 7 = Fail (Blue --> More or Less than  +/- Threshold of Compared with Golden Sample)\r\n");

	
    fprintf(file, "             ,,Fail Parameter: , A = Voltage vs Target\r\n");
    fprintf(file, "                             ,,, B = Voltage vs Golden Sample\r\n");
    fprintf(file, "                             ,,, C = Calibration vs Golden Sample\r\n");

    fprintf(file, "Node Constraints\r\n");
	fprintf(file, "Target Voltage Threshold (%%), Post, %.0f, %.2fV, Neg, %.0f, %.2fV\r\n", config->pointConfig.targetMutl, data->getTargetVoltage()*(1+config->pointConfig.targetMutl/100.0f), config->pointConfig.negTargetMutl, data->getTargetVoltage()*(1+config->pointConfig.negTargetMutl/100.0f));
	fprintf(file, "Calibration Threshold with Golden Sample (%%), Post, %.0f,  Neg, %.0f\r\n", config->pointConfig.calib, config->pointConfig.negCalib);
	fprintf(file, "Voltage Threshold  with Golden Sample (%%), Post, %.0f,  Neg, %.0f\r\n", config->pointConfig.mutl , config->pointConfig.negMutl);

	fprintf(file, "Line Constraints\r\n");
	fprintf(file, "Calib Time (%%), %.0f, Voltage, %.0f\r\n", config->lineConfig.calibTime , config->lineConfig.voltage);	
	fprintf(file, "Out of Spec Node (%%), %.0f\r\n", config->lineConfig.outOfSpecNode );	
	fprintf(file, "Max Loop, %.0f,  Min Loop, %.0f\r\n\r\n", config->lineConfig.maxLoop, config->lineConfig.minLoop);

	fprintf(file, "X Inverse,%d,Y Inverse,%d,\r\n\r\n", config->orderConfig.InvXorder, config->orderConfig.InvYorder);

	
    fprintf(file, "Width,%d,", width);
    for ( int devidx = 0; devidx < 1; devidx++ )
    {
        fprintf(file, "Master,%d,", width);
    }
    fprintf(file, "\r\n");

    fprintf(file, "Height,%d,", height);
    for ( int devidx = 0; devidx < 1; devidx++ )
    {
        fprintf(file, "Master,%d,", height);
    }
    fprintf(file, "\r\n");

    fprintf(file, "Button,%d,,,\r\n",  btnChannel);
    fprintf(file, "\r\n");

	fprintf(file, "Table%d:,CalibLoopcnt\n", LOOPID);
    OpenShortResult::fprintData(file, data, result, config, LOOPID);

    fprintf(file, "\nTable%d:,CalibSampleTime\n", CYCLEID);
    OpenShortResult::fprintData(file, data, result, config, CYCLEID);

    fprintf(file, "\nTable%d:,Voltage,(Target Voltage,%.3f,)\n", VoltageID, data->getTargetVoltage());
    OpenShortResult::fprintData(file, data, result, config, VoltageID);
	
	fprintf(file, "\nTable%d:,Result(Fail Parameter)\n", ResultID);
	OpenShortResult::fprintData(file, data, result, config, ResultID);
  
    fprintf(file, "\r\n");
    fprintf(file, "Line Result Comment: , 0x00000000 = PASS, 0x00000001 = SHORT, 0x00000002 = SHORT_LOOP, 0x00000003 = EXTREME_DIF\r\n");
    fprintf(file, "   Line,  LineResult\r\n");

    for ( int i = 0; i < width; i++)
    {
        fprintf(file, "x%06d,  0x%08x\r\n", (i+1), result->getXLineResult(i));
    }

    for ( int i = 0; i < height; i++)
    {
        fprintf(file, "y%06d,  0x%08x\r\n", (i+1), result->getYLineResult(i));
    }

    fprintf(file, "\r\n");

    // new add -------------------------------------------------------------
    // BY chip -------------------------------------------------------------

    fprintf(file, "X NG Channel,");
    for ( int i = 0; i < width; i++)
    {
        //fprintf(file, "x%06d,  0x%08x\r\n", (i+1), result->getXLineResult(i));
        if(result->getXLineResult(i) == LINE_PASS)
        {
        } else if(result->getXLineResult(i) == LINE_SHORT)
        {
            fprintf(file, "X%d(SHORT),", (i+1));
        } else if(result->getXLineResult(i) == LINE_SHORT_LOOP)
        {
            fprintf(file, "X%d(SHORT_LOOP),", (i+1));
        } else if(result->getXLineResult(i) == LINE_EXTREME_DIF)
        {
            fprintf(file, "X%d(EX),", (i+1));
        } else if(result->getXLineResult(i) == LINE_NEARFAR_SHORT)
        {
            fprintf(file, "X%d(NF),", (i+1));
        }
    }
    fprintf(file, "\r\n");

    fprintf(file, "Y NG Channel,");
    for ( int i = 0; i < height; i++)
    {
        //fprintf(file, "y%06d,  0x%08x\r\n", (i+1), result->getYLineResult(i));
        if(result->getYLineResult(i) == LINE_PASS)
        {
        } else if(result->getYLineResult(i) == LINE_SHORT)
        {
            fprintf(file, "%d(SHORT),", (i+1));
        } else if(result->getYLineResult(i) == LINE_SHORT_LOOP)
        {
            fprintf(file, "%d(SHORT_LOOP),", (i+1));
        } else if(result->getYLineResult(i) == LINE_EXTREME_DIF)
        {
            fprintf(file, "%d(EX),", (i+1));
        } else if(result->getYLineResult(i) == LINE_NEARFAR_SHORT)
        {
            if((result->lostSide & LOST_LEFT) != 0)
            {
                fprintf(file, "%d_%d(NF),", (i+1), 0);
            } else if((result->lostSide & LOST_RIGHT) != 0)
            {
                fprintf(file, "%d_%d(NF),", (i+1), 1);
            } else
            {
                fprintf(file, "%d(NF),", i+1);
            }
        }
    }

    fprintf(file, "\r\n");

    //2D_Mutual_ExtraCheckResult_#1.csv ------------------------------------------------

    // MutualShortExtraCheck, begin
    fprintf(file, "\r\n");
    fprintf(file, "\n2D_Mutual_ExtraCheckResult_#1.csv\n");

    //header---------------------------------------------------------------------------- 
    fprintf( file, "MutualShortExtraCheck Constraints,\n" );
    fprintf( file, "Enable TX Extra Check, %d, Enable RX Extra Check, %d, Enable Auto NG Check, %d, \n", 
                   (int) config->lineExtraConfig.enableTXExtraCheck, (int) config->lineExtraConfig.enableRXExtraCheck, (int) config->lineExtraConfig.enableAutoNgCheck);
    fprintf( file, "Cycle Threshold (%%), TX High, %d, TX Low, %d, ", (int) config->lineExtraConfig.TXCycleDiffHighPercent, (int) config->lineExtraConfig.TXCycleDiffLowPercent);
    fprintf( file, "RX High, %d, RX Low, %d, \n", (int) config->lineExtraConfig.RXCycleDiffHighPercent, (int) config->lineExtraConfig.RXCycleDiffLowPercent);
    fprintf( file, "Out of Spec Node (%%), %d, \n", (int) config->lineExtraConfig.cycleOutOfSpecNodePercent);
    fprintf( file, "MutualShortExtraCheck By TP, %d, Detect Short Only, %d, \n", (int) config->lineExtraConfig.MutualShortExtraCheckByTP, (int) config->lineExtraConfig.detectShortOnly);
    fprintf( file, "\n" );

    fprintf( file, "MutualNearFarCheck Constraints,\n" );
    fprintf( file, "COB Connector Y Type, %d, Enable Auto Near Far Check, %d, \n", (int) config->lineNearFarConfig.enableCOBConnectorYType, (int) config->lineNearFarConfig.enableAutoNearFarCheck);
    fprintf( file, "Cycle Difference Threshold (%%), TX, %d, RX, %d, \n", (int) config->lineNearFarConfig.cycleDifferencePercentTx, (int) config->lineNearFarConfig.cycleDifferencePercentRx);
    fprintf( file, "Out of Spec Channel, %d, \n", (int) config->lineNearFarConfig.cycleOutOfChannels);
    fprintf( file, "Cycle Avg Count Number, TX, %d, RX, %d, \n", (int) config->lineNearFarConfig.cycleAvgCountNumberTx, (int) config->lineNearFarConfig.cycleAvgCountNumberRx);
    fprintf( file, "\n" );

    // printf table: CycleTxDiff ------------------------------------------------
    fprintf(file, "\nCycleTxDiff,\n");
    OpenShortResult::fprintData(file, data, result, config, CycleTxDiffID); 

    fprintf(file, "\r\n");
    if((int)config->lineExtraConfig.enableTXExtraCheck == 1)
    {
        fprintf(file, "TX enable,");
    } else 
    {
        fprintf(file, "TX disable,");
    }

    // printf table: TXPointShortResult ------------------------------------------------
    fprintf(file, "\nTXPointShortResult, PASS 1, FAIL 0,\n");
    OpenShortResult::fprintData(file, data, result, config, TXPointShortResultID); 

    // printf table: CycleRxDiff ------------------------------------------------
    fprintf(file, "\nCycleRxDiff,\n");
    OpenShortResult::fprintData(file, data, result, config, CycleRxDiffID);

    fprintf(file, "\r\n");
    if((int)config->lineExtraConfig.enableRXExtraCheck == 1)
    {
        fprintf(file, "RX enable,");
    } else 
    {
        fprintf(file, "RX disable,");
    }

    // printf table: RXPointShortResult ------------------------------------------------
    fprintf(file, "\nRXPointShortResult, PASS 1, FAIL 0,\n");
    OpenShortResult::fprintData(file, data, result, config, RXPointShortResultID); 

    // MutualShortExtraCheck, end

    fprintf(file, "\r\n");
    fprintf(file, "\r\n");

    // MutualNearFarCheck, begin
    // TX ------------------------------------------------------------------------------
    fprintf(file, "CheckMutualNearFarTxValue,\n");
    for(int i = 0; i < height; i++)
    {
        fprintf(file, "%5c%3d,", 'Y', i+1);
    }
    fprintf(file, "\n");
    for(int i = 0; i < height; i++)
    {
        fprintf(file, "%8.3f,", result->getYLineNearFarValue(i));
    }
    fprintf(file, "\n");
    fprintf(file, "CheckMutualNearFarTxResult, PASS 1, FAIL 0,\n");
    for(int i = 0; i < height; i++)
    {
        switch(result->getYLineNearFarResult(i))
        {
        case LINE_PASS:
            fprintf(file, "%8d,", 1);
            break;
        case LINE_NEARFAR_SHORT:
            fprintf(file, "%8d,", 0);
            break;
        default:
            fprintf(file, "%8d,", result->getYLineNearFarResult(i));
            break;
        }
    }
    fprintf(file, "\r\n");
    fprintf(file, "\r\n");

    // RX ------------------------------------------------------------------------------
    fprintf(file, "CheckMutualNearFarRxValue,\n");
    for(int i = 0; i < width; i++)
    {
        fprintf(file, "%5c%3d,", 'X', i+1);
    }
    fprintf(file, "\n");
    for(int i = 0; i < width; i++)
    {
        fprintf(file, "%8.3f,", result->getXLineNearFarValue(i));
    }
    fprintf(file, "\n");
    fprintf(file, "CheckMutualNearFarRxResult, PASS 1, FAIL 0,\n");
    for(int i = 0; i < width; i++)
    {
        switch(result->getXLineNearFarResult(i))
        {
        case LINE_PASS:
            fprintf(file, "%8d,", 1);
            break;
        case LINE_NEARFAR_SHORT:
            fprintf(file, "%8d,", 0);
            break;
        default:
            fprintf(file, "%8d,", result->getXLineNearFarResult(i));
            break;
        }
    }
    // MutualNearFarCheck, end

    return true;
}


