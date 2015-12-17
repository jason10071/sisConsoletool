#include "MultiOpenShortResult.h"
#include "MultiOpenShortData.h"
#include "OpenShortConfig.h"

#include <string.h>

MultiOpenShortResult::MultiOpenShortResult() :
    mWidthTP(0)
    ,mHeightTP(0)
    ,mChipNum(0)
    ,mResults(0)
    ,mChipOrderIndexs(0)
    ,mPointTpResult(0)
    ,mLineTpResult(0)
    ,mLineTpNearFarValue(0)
    ,mLineTpNearFarResult(0)
    //,mPointTpNG(0)
    ,mLineTpNG(0)
    ,lostSide(0)
{
    setChipNum(1);
}

MultiOpenShortResult::MultiOpenShortResult(int chipNum) :
    mWidthTP(0)
    ,mHeightTP(0)
    ,mChipNum(chipNum)
    ,mResults(0)
    ,mChipOrderIndexs(0) 
    ,mPointTpResult(0)
    ,mLineTpResult(0)
    ,mLineTpNearFarValue(0)
    ,mLineTpNearFarResult(0)
    //,mPointTpNG(0)
    ,mLineTpNG(0)
    ,lostSide(0)
{
    if(chipNum < 1)
    {
        printf("minimum chipNum is 1.\n");
        chipNum = 1;
    }
    setChipNum(chipNum);
}

MultiOpenShortResult::~MultiOpenShortResult()
{
    delete [] mResults;
    delete [] mChipOrderIndexs;
    delete [] mLineTpNearFarValue;
    delete [] mLineTpNearFarResult;
}

// do after init single-chip
void
MultiOpenShortResult::init(MultiOpenShortData* deviceData)
{
    //mPointTpNG = 0;
    mLineTpNG = 0;

    if(mLineTpResult != 0)
    {
        delete [] mPointTpResult;
        delete [] mLineTpResult;
        delete [] mLineTpNearFarValue;
        delete [] mLineTpNearFarResult;

        mPointTpResult = 0;
        mLineTpResult = 0;
        mLineTpNearFarValue = 0;
        mLineTpNearFarResult = 0;
    }

    mWidthTP = deviceData->getWidth();
    mHeightTP = deviceData->getHeight();

    mPointTpResult = new unsigned int[mWidthTP * mHeightTP];
    memset(mPointTpResult, 0, sizeof(unsigned int) * mWidthTP * mHeightTP);

    // getWidth() = RX, getHeight() = TX
    mLineTpResult = new unsigned int[mWidthTP + mHeightTP];
    memset(mLineTpResult, 0, sizeof(unsigned int) * (mWidthTP + mHeightTP));

    mLineTpNearFarValue = new float[mWidthTP + mHeightTP];
    memset(mLineTpNearFarValue, 0, sizeof(float) * (mWidthTP + mHeightTP));

    mLineTpNearFarResult = new unsigned int[mWidthTP + mHeightTP];
    memset(mLineTpNearFarResult, 0, sizeof(unsigned int) * (mWidthTP + mHeightTP));

}

void
MultiOpenShortResult::setChipNum(int chipNum)
{
    mChipNum = chipNum;

    if(mResults != 0)
    {
        delete [] mResults;
        delete [] mChipOrderIndexs;
    }
    mResults = new OpenShortResult[mChipNum];

    mChipOrderIndexs = new int[chipNum];

    for(int i=0;i<chipNum;i++)
    {
        mChipOrderIndexs[i] = i;
    }

}

int
MultiOpenShortResult::getChipNum()
{
    return mChipNum;
}

bool
MultiOpenShortResult::setOrderInfo(OpenShortConfig* config)
{

    bool result = true;
    if(config->orderConfig.reArrange)
    {
        for(int i=0;i<mChipNum;i++)
        {

            if(i < OrderOpenShortConfig::MAX_CHIP)
            {
                if(config->orderConfig.arrangeIndexs[i] < mChipNum)
                {
                    setChipOrderIndex(i, config->orderConfig.arrangeIndexs[i]);
                }
                else
                {
                    setChipOrderIndex(i, config->orderConfig.arrangeIndexs[i]);
                    printf("chip order arrangement is invalid, the assigiment is out of current chip num\n");
                    result = false;
                }
            }
            else
            {
                setChipOrderIndex(i, OrderOpenShortConfig::INDEX_MASTER);
                printf("chip order arrangement is invalid, the assigiment is out of current chip num\n");
                result = false;
            }
        }
    }

    return result;
}

void
MultiOpenShortResult::setChipOrderIndex(int index, int orderIndex)
{
    mChipOrderIndexs[index] = orderIndex;
}

int
MultiOpenShortResult::getChipOrderIndex(int index)
{
    return mChipOrderIndexs[index];
}

OpenShortResult*
MultiOpenShortResult::getResult(int index)
{
    if(index < mChipNum && index >= 0)
    {
        return &(mResults[index]);
    }
    else
    {
        return NULL;
    }
}

void
MultiOpenShortResult::queryMappingSingle(int &devIdx, int &x, int &y)
{
    if(mChipNum == 1)
    {
        devIdx = 0;
    }
    else
    {
        devIdx = 0;
        for(devIdx = 0; devIdx < mChipNum; devIdx++)
        {
            int chipHeight = getHeight(devIdx);
            if(x >= chipHeight)
            {
                x -= chipHeight;
            }
            else
            {
                break;
            }
        }
        // x, y switch;
        int temp = x;
        x = y;
        y = temp;
    }

}

int
MultiOpenShortResult::getWidth()
{
    int width = 0;
    if(mChipNum == 1)
    {
        width += mResults[0].getWidth();
    }
    else
    {
        for(int index = 0; index < mChipNum; index++)
        {
            width += mResults[index].getHeight();
        }
    }
    return width;
}

int
MultiOpenShortResult::getWidth(int index)
{
    if(index < mChipNum && index >= 0)
    {
        return mResults[index].getWidth();
    }
    else
    {
        return 0;
    }

}

int
MultiOpenShortResult::getHeight()
{
    int height = 0;
    if(mChipNum == 1)
    {
        height = mResults[0].getHeight();
    }
    else if(mChipNum < 1)
    {
        height = 0;
    }
    else
    {
        height =  mResults[0].getWidth();
    }
    return height;
}

int
MultiOpenShortResult::getHeight(int index)
{
    if(index < mChipNum && index >= 0)
    {
        return mResults[index].getHeight();
    }
    else
    {
        return 0;
    }
}

int
MultiOpenShortResult::getBtnChannel()
{
    return 0;
}

int
MultiOpenShortResult::getSecondNG()
{
    int secondNG = 0;
    for(int index = 0; index < mChipNum; index++)
    {
        secondNG += mResults[index].getSecondNG();
    }
    return secondNG;
}


int
MultiOpenShortResult::getPointNG()
{
    int pointNG = 0;
    for(int index = 0; index < mChipNum; index++)
    {
        pointNG += mResults[index].getPointNG();
    }
    return pointNG;
}

int
MultiOpenShortResult::getLineNG()
{
    int lineNG = 0;
    for(int index = 0; index < mChipNum; index++)
    {
        lineNG += mResults[index].getLineNG();
    }
    return lineNG;
}

// MutualShortExtraCheck, begin
float
MultiOpenShortResult::getCycleTxDiff(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    if(mChipNum == 1)
    {
        return mResults[devIdx].getCycleTxDiff(x, y);
    }
    else
    {
        //x/y switch
        return mResults[devIdx].getCycleRxDiff(x, y);
    }
    
}
unsigned int
MultiOpenShortResult::getTXPointShortResult(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    if(mChipNum == 1)
    {
        return mResults[devIdx].getTXPointShortResult(x, y);
    }
    else
    {
        //x/y switch
        return mResults[devIdx].getRXPointShortResult(x, y);
    }
}

float
MultiOpenShortResult::getCycleRxDiff(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    if(mChipNum == 1)
    {
        return mResults[devIdx].getCycleRxDiff(x, y);
    }
    else
    {
        //x/y switch
        return mResults[devIdx].getCycleTxDiff(x, y);
    }
}
unsigned int
MultiOpenShortResult::getRXPointShortResult(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    if(mChipNum == 1)
    {
        return mResults[devIdx].getRXPointShortResult(x, y);
    }
    else
    {
        //x/y switch
        return mResults[devIdx].getTXPointShortResult(x, y);
    }
}
// MutualShortExtraCheck, end

// MutualShortExtraCheckByTP, begin
/*
void
MultiOpenShortResult::setPointTpNG(int ngNum)
{
    mPointTpNG = ngNum;
}
*/

void
MultiOpenShortResult::setLineTpNG(int ngNum)
{
    mLineTpNG = ngNum;
}

/*
int
MultiOpenShortResult::getPointTpNG()
{
    return mPointTpNG;
}
*/

int
MultiOpenShortResult::getLineTpNG()
{
    return mLineTpNG;
}

void
MultiOpenShortResult::setPointTpResult(int x, int y, unsigned int result)
{

    mPointTpResult[x * mHeightTP + y] = result;
}

void
MultiOpenShortResult::setXLineTpResult(int x, unsigned int result)
{
    mLineTpResult[x] = result;
}

void
MultiOpenShortResult::setYLineTpResult(int y, unsigned int result)
{
    mLineTpResult[mWidthTP + y] = result;
}

unsigned int
MultiOpenShortResult::getPointTpResult(int x, int y)
{
    return mPointTpResult[x * mHeightTP + y];
}


unsigned int
MultiOpenShortResult::getXLineTpResult(int x)
{
    return mLineTpResult[x];
}

unsigned int
MultiOpenShortResult::getYLineTpResult(int y)
{
    return mLineTpResult[mWidthTP + y];
}
// MutualShortExtraCheckByTP, end

// MutualNearFarCheckByTP, begin
void
MultiOpenShortResult::setXLineTpNearFarValue(int x, float result)
{
    mLineTpNearFarValue[x] = result;
}

void
MultiOpenShortResult::setYLineTpNearFarValue(int y, float result)
{
    mLineTpNearFarValue[mWidthTP + y] = result;
}

float
MultiOpenShortResult::getXLineTpNearFarValue(int x)
{
    return mLineTpNearFarValue[x];
}

float
MultiOpenShortResult::getYLineTpNearFarValue(int y)
{
    return mLineTpNearFarValue[mWidthTP + y];
}

void
MultiOpenShortResult::setXLineTpNearFarResult(int x, unsigned int result)
{
    mLineTpNearFarResult[x] = result;
}

void
MultiOpenShortResult::setYLineTpNearFarResult(int y, unsigned int result)
{
    mLineTpNearFarResult[mWidthTP + y] = result;
}

unsigned int
MultiOpenShortResult::getXLineTpNearFarResult(int x)
{
    return mLineTpNearFarResult[x];
}

unsigned int
MultiOpenShortResult::getYLineTpNearFarResult(int y)
{
    return mLineTpNearFarResult[mWidthTP + y];
}
// MutualNearFarCheckByTP, end

unsigned int
MultiOpenShortResult::getPointResult(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mResults[devIdx].getPointResult(x, y);
}

void
MultiOpenShortResult::setPointResult(int x, int y, unsigned int result)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mResults[devIdx].setPointResult(x, y, result);
}

void
MultiOpenShortResult::appendPointResult(int x, int y, unsigned int result)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mResults[devIdx].appendPointResult(x, y, result);
}

int
MultiOpenShortResult::getXChipCount()
{
    return mChipNum;
}

int
MultiOpenShortResult::getYChipCount()
{
    if(mChipNum > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned int
MultiOpenShortResult::getXLineResult(int x)
{
    unsigned result = 0;
    int devIdx = 0;
    int y = 0;
    queryMappingSingle(devIdx, x, y);

    if(mChipNum == 1)
    {
        result = mResults[devIdx].getXLineResult(x);
    }
    else
    {
        //x/y switch
        result = mResults[devIdx].getYLineResult(y);
    }
    return result;    
}

unsigned int
MultiOpenShortResult::getYLineResult(int y)
{
    unsigned result = 0;
    if(mChipNum == 1)
    {
        result = mResults[0].getYLineResult(y);
    }
    else
    {
        //x/y switch
        for(int devIdx = mChipNum - 1; devIdx >= 0; devIdx--)
        {
            result = result << MULTI_CHIP_MASK_LEN;
            unsigned lineResult = mResults[devIdx].getXLineResult(y);
            result |= lineResult;
        }
    }

    return result;    
}

unsigned int
MultiOpenShortResult::getYLineResult(int devIdx, int y)
{
    unsigned result = 0;
    if(mChipNum == 1)
    {
        result = mResults[0].getYLineResult(y);
    }
    else
    {
        //x/y switch
        result = mResults[devIdx].getXLineResult(y);
    }

    return result;    
}

void
MultiOpenShortResult::dumpResult()
{
    int width = getWidth();
    int height = getHeight();

    int lineNG = getLineNG();
    int pointNG = getPointNG();

    unsigned int lineResult = 0;

    if(lineNG != 0)
    {
        int xCount = getXChipCount();            
        int yCount = getYChipCount();
        for(int i=0; i<width; i++)
        {
            lineResult = getXLineResult(i);
            for(int count=0; count < xCount; count++)
            {
                unsigned int partialResult = lineResult && MULTI_CHIP_MASK;
                if(partialResult  != LINE_PASS)
                {
                    printf("line x %d is suspicious, state is %08x.\n", i+1, getXLineResult(i));
                    break;
                } 
                else
                {
                lineResult = lineResult >> MULTI_CHIP_MASK_LEN;
                }
            }          
        }

        for(int j=0; j<height; j++)
        {
            lineResult = getYLineResult(j);
            for(int count=0; count < yCount; count++)
            {
                unsigned int partialResult = lineResult && MULTI_CHIP_MASK;
                if(partialResult  != LINE_PASS)
                {
                    printf("line y %d is suspicious, state is %08x.\n", j+1, getYLineResult(j));
                    break;
                }
                else
                {
                lineResult = lineResult >> MULTI_CHIP_MASK_LEN;
                }
            }          
        }
    }



    if(pointNG !=0)
    {
        for(int i=0; i<width; i++)
        {
            for(int j=0; j<height; j++)
            {
                unsigned int pointResult = getPointResult(i, j);
                if(getPointResult(i, j) != POINT_PASS)
                {
                    printf("point %d, %d is suspicious, state is %08x.\n", i+1, j+1, pointResult);
                }                                  
            }    
        }
    }

}

void
MultiOpenShortResult::fprintMultiData(FILE *fp2D, MultiOpenShortData* data, MultiOpenShortResult *result, OpenShortConfig *config, int tableID)
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
MultiOpenShortResult::saveMultiOpenShortResult(FILE *file, MultiOpenShortData* multiData, MultiOpenShortResult *multiResult, OpenShortConfig *config)
{
    if(!file)
    {
        printf("null file descriptor\n");
        return false;
    }

    if(multiData->getChipNum() != multiResult->getChipNum())
    {
        printf("chip num is not corresponding\n");
        return false;
    }

    int chipNum = multiData->getChipNum();

    int width = multiData->getWidth();
    int height = multiData->getHeight();
    int btnChannel = multiData->getBtnChannel();
    // char reasonBuf[255] = {0};

    fprintf(file, "NG Num,%6d, Result Comment: , 1 = Pass (Green --> Under +/- Threshold of Target or Compared with Golden Sample)\r\n", multiResult->getPointNG() + (config->lineExtraConfig.MutualShortExtraCheckByTP == 0 ? multiResult->getLineNG() : multiResult->getLineTpNG())  );
    fprintf(file, "Node NG,%5d,                 , 0 = Fail (Purple --> Less than Target Voltage)\r\n", multiResult->getPointNG());
    fprintf(file, "Line NG,%5d,                 , 4 = Fail (Red --> More than Target Voltage)\r\n", (config->lineExtraConfig.MutualShortExtraCheckByTP == 0 ? multiResult->getLineNG() : multiResult->getLineTpNG())  );

    fprintf(file, "                             ,,, 7 = Fail (Blue --> More or Less than  +/- Threshold of Compared with Golden Sample)\r\n");


	fprintf(file, "             ,,Fail Parameter: , A = Voltage vs Target\r\n");
    fprintf(file, "                             ,,, B = Voltage vs Golden Sample\r\n");
    fprintf(file, "                             ,,, C = Calibration vs Golden Sample\r\n");

	    
	fprintf(file, "Node Constraints\n");
	fprintf(file, "Target Voltage Threshold (%%), Post, %.0f, %.2fV, Neg, %.0f, %.2fV\r\n", config->pointConfig.targetMutl, multiData->getTargetVoltage(0,0)*(1+config->pointConfig.targetMutl/100.0f), config->pointConfig.negTargetMutl, multiData->getTargetVoltage(0,0)*(1+config->pointConfig.negTargetMutl/100.0f));
	fprintf(file, "Calibration Threshold with Golden Sample (%%), Post, %.0f,  Neg, %.0f\r\n", config->pointConfig.calib, config->pointConfig.negCalib);
	fprintf(file, "Voltage Threshold  with Golden Sample (%%), Post, %.0f,  Neg, %.0f\r\n", config->pointConfig.mutl , config->pointConfig.negMutl); 

	fprintf(file, "Line Constraints\r\n");
	fprintf(file, "Calib Time (%%), %.0f, Voltage, %.0f\r\n", config->lineConfig.calibTime , config->lineConfig.voltage);	
	fprintf(file, "Out of Spec Node (%%), %.0f\r\n", config->lineConfig.outOfSpecNode );	
	fprintf(file, "Max Loop, %.0f,  Min Loop, %.0f\r\n\r\n", config->lineConfig.maxLoop, config->lineConfig.minLoop);
    
	fprintf(file, "X Inverse,%d,Y Inverse,%d,\r\n", config->orderConfig.InvXorder, config->orderConfig.InvYorder);

	int accWidth = 0;
	
	if(chipNum > 1)
    {
        for ( int devidx = 0; devidx < chipNum; devidx++ )
        {            
            int chipOrderIndex = multiData->getChipOrderIndex(devidx);
			
			fprintf(file, "X%d~X%d,", accWidth + 1, accWidth + multiData->getHeight(devidx) );
			accWidth += multiData->getHeight(devidx);
			
            if(chipOrderIndex == OrderOpenShortConfig::INDEX_MASTER)
            {
                fprintf(file, "Master,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE0)
            {
                fprintf(file, "Slave0,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE1)
            {
                fprintf(file, "Slave1,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE2)
            {
                fprintf(file, "Slave2,");
            }
           else
            {
                fprintf(file, "Slave%d,", (chipOrderIndex-1));
            }           
        }
    }

	fprintf(file, "\r\n\r\n");
	
    fprintf(file, "Width,%d,", width);

    if(chipNum > 1)
    {
        for ( int devidx = 0; devidx < chipNum; devidx++ )
        {
            int chipOrderIndex = multiData->getChipOrderIndex(devidx);
            
            if(chipOrderIndex == OrderOpenShortConfig::INDEX_MASTER)
            {
                fprintf(file, "Master,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE0)
            {
                fprintf(file, "Slave0,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE1)
            {
                fprintf(file, "Slave1,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE2)
            {
                fprintf(file, "Slave2,");
            }
           else
            {
                fprintf(file, "Slave%d,", (chipOrderIndex-1));
            }
           fprintf(file, "%d,", multiData->getWidth(devidx));   
        }
    }
    fprintf(file, "\r\n");
    fprintf(file, "Height,%d,", height);

    if(chipNum > 1)
    {
        for ( int devidx = 0; devidx < chipNum; devidx++ )
        {
            int chipOrderIndex = multiData->getChipOrderIndex(devidx);
    
            if(chipOrderIndex == OrderOpenShortConfig::INDEX_MASTER)
            {
                fprintf(file, "Master,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE0)
            {
                fprintf(file, "Slave0,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE1)
            {
                fprintf(file, "Slave1,");
            }
           else if(chipOrderIndex == OrderOpenShortConfig::INDEX_SLAVE2)
            {
                fprintf(file, "Slave2,");
            }
           else
            {
                fprintf(file, "Slave%d,", (chipOrderIndex-1));
            }
           fprintf(file, "%d,", multiData->getHeight(devidx));            
        }
    }
    fprintf(file, "\r\n");
    fprintf(file, "Button,%d,,,\r\n",  btnChannel);

    fprintf(file, "\r\n");

	fprintf(file, "Table%d:,CalibLoopcnt\n", LOOPID);
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, LOOPID);

    fprintf(file, "\nTable%d:,CalibSampleTime\n", CYCLEID);
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, CYCLEID);

    fprintf(file, "\nTable%d:,Voltage,(Target Voltage,%.3f,)\n", VoltageID, multiData->getTargetVoltage(0,0));
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, VoltageID);
	
	fprintf(file, "\nTable%d:,Result(Fail Parameter)\n", ResultID);
	MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, ResultID);    
	
    fprintf(file, "\r\n");
    fprintf(file, "Line Result Comment: , 0x00000000 = PASS, 0x00000001 = SHORT, 0x00000002 = SHORT_LOOP, 0x00000003 = EXTREME_DIF\r\n");
    fprintf(file, "   Line,  LineResult\r\n");

    for ( int i = 0; i < width; i++)
    {
        fprintf(file, "x%06d,  0x%08x\r\n", (i+1), multiResult->getXLineResult(i));
    }

    for ( int i = 0; i < height; i++)
    {
        fprintf(file, "y%06d,  0x%08x\r\n", (i+1), multiResult->getYLineResult(i));
    }

    fprintf(file, "\r\n");

    // new add -------------------------------------------------------------

    if(config->lineExtraConfig.MutualShortExtraCheckByTP == 0)
    {
    	// BY chip -------------------------------------------------------------
        fprintf(file, "X NG Channel,");
        for ( int i = 0; i < width; i++)
        {
            //fprintf(file, "x%06d,  0x%08x\r\n", (i+1), multiResult->getXLineResult(i));
            if(multiResult->getXLineResult(i) == LINE_PASS)
            {
            }
            else if(multiResult->getXLineResult(i) == LINE_SHORT)
            {
                fprintf(file, "X%d(SHORT),", (i+1));
            }
            else if(multiResult->getXLineResult(i) == LINE_SHORT_LOOP)
            {
                fprintf(file, "X%d(SHORT_LOOP),", (i+1));
            }
            else if(multiResult->getXLineResult(i) == LINE_EXTREME_DIF)
            {
                fprintf(file, "X%d(EX),", (i+1));
            }

            if(multiResult->getXLineTpResult(i) == LINE_NEARFAR_SHORT)
            {
                fprintf(file, "X%d(NF),", (i+1));
            }
        }
        fprintf(file, "\r\n");

        fprintf(file, "Y NG Channel,");
        for ( int i = 0; i < height; i++)
        {
            //fprintf(file, "y%06d,  0x%08x\r\n", (i+1), multiResult->getYLineResult(i));
            for(int devIdx = 0; devIdx < chipNum; devIdx++)
            {
                if(multiResult->getYLineResult(devIdx, i) == LINE_PASS)
                {
                }
                else if(multiResult->getYLineResult(devIdx, i) == LINE_SHORT)
                {
                    fprintf(file, "%d_%d(SHORT),", (i+1), devIdx);
                }
                else if(multiResult->getYLineResult(devIdx, i) == LINE_SHORT_LOOP)
                {
                    fprintf(file, "%d_%d(SHORT_LOOP),", (i+1), devIdx);
                }
                else if(multiResult->getYLineResult(devIdx, i) == LINE_EXTREME_DIF)
                {
                    fprintf(file, "%d_%d(EX),", (i+1), devIdx);
                }
            }

            if(multiResult->getYLineTpResult(i) == LINE_NEARFAR_SHORT)
            {
                if((multiResult->lostSide & LOST_LEFT) != 0)
                {
                    fprintf(file, "%d_%d(NF),", (i+1), 0);
                }
                else if((multiResult->lostSide & LOST_RIGHT) != 0)
                {
                    fprintf(file, "%d_%d(NF),", (i+1), chipNum-1);
                }
                else
                {
                    fprintf(file, "%d(NF),", i+1);
                }
            }
        }

        fprintf(file, "\r\n");

        fprintf(file, "\r\n");
    }
    else
    {
        // MutualShortExtraCheckByTP, begin
        // BY TP -------------------------------------------------------------
        fprintf(file, "X NG Channel,");
        for ( int i = 0; i < width; i++)
        {
            if(multiResult->getXLineTpResult(i) == LINE_PASS)
            {
            }
            else if(multiResult->getXLineTpResult(i) == LINE_SHORT)
            {
                fprintf(file, "X%d(SHORT),", (i+1));
            }
            else if(multiResult->getXLineTpResult(i) == LINE_SHORT_LOOP)
            {
                fprintf(file, "X%d(SHORT_LOOP),", (i+1));
            }
            else if(multiResult->getXLineTpResult(i) == LINE_EXTREME_DIF)
            {
                fprintf(file, "X%d(EX),", (i+1));
            }
            else if(multiResult->getXLineTpResult(i) == LINE_NEARFAR_SHORT)
            {
                fprintf(file, "X%d(NF),", (i+1));
            }
        }
        fprintf(file, "\r\n");

        fprintf(file, "Y NG Channel,");
        for ( int i = 0; i < height; i++)
        {
            if(multiResult->getYLineTpResult(i) == LINE_PASS)
            {
            }
            else if(multiResult->getYLineTpResult(i) == LINE_SHORT)
            {
                fprintf(file, "%d(SHORT),", (i+1));
            }
            else if(multiResult->getYLineTpResult(i) == LINE_SHORT_LOOP)
            {
                fprintf(file, "%d(SHORT_LOOP),", (i+1));
            } else if(multiResult->getYLineTpResult(i) == LINE_EXTREME_DIF)
            {
                fprintf(file, "%d(EX),", (i+1));
            }
            else if(multiResult->getYLineTpResult(i) == LINE_NEARFAR_SHORT)
            {
                if((multiResult->lostSide & LOST_LEFT) != 0)
                {
                    fprintf(file, "%d_%d(NF),", (i+1), 0);
                }
                else if((multiResult->lostSide & LOST_RIGHT) != 0)
                {
                    fprintf(file, "%d_%d(NF),", (i+1), chipNum-1);
                }
                else
                {
                    fprintf(file, "%d(NF),", i+1);
                }
            }
        }
        // MutualShortExtraCheckByTP, end
    }






    //2D_Mutual_ExtraCheckResult_#1.csv ------------------------------------------------

    // MutualShortExtraCheck, begin
    fprintf(file, "\r\n");
    fprintf(file, "\n2D_Mutual_ExtraCheckResult_#1.csv\n");

    //header---------------------------------------------------------------------------- 
    fprintf( file, "MutualShortExtraCheck Constraints,\n" );
    fprintf( file, "Enable TX Extra Check, %d, Enable RX Extra Check, %d, Enable Auto NG Check, %d, \n", 
                   (int) config->lineExtraConfig.enableRXExtraCheck, (int) config->lineExtraConfig.enableTXExtraCheck, (int) config->lineExtraConfig.enableAutoNgCheck); // Multi: x/y switch

    fprintf( file, "Cycle Threshold (%%), TX High, %d, TX Low, %d, ", (int) config->lineExtraConfig.RXCycleDiffHighPercent, (int) config->lineExtraConfig.RXCycleDiffLowPercent); // Multi: x/y switch
    fprintf( file, "RX High, %d, RX Low, %d, \n", (int) config->lineExtraConfig.TXCycleDiffHighPercent, (int) config->lineExtraConfig.TXCycleDiffLowPercent); // Multi: x/y switch

    fprintf( file, "Out of Spec Node (%%), %d, \n", (int) config->lineExtraConfig.cycleOutOfSpecNodePercent);
    fprintf( file, "MutualShortExtraCheck By TP, %d, Detect Short Only, %d, \n", (int) config->lineExtraConfig.MutualShortExtraCheckByTP, (int) config->lineExtraConfig.detectShortOnly);
    fprintf( file, "\n" );

    fprintf( file, "MutualNearFarCheck Constraints,\n" );
    fprintf( file, "COB Connector Y Type, %d, Enable Auto Near Far Check, %d, \n", (int) config->lineNearFarConfig.enableCOBConnectorYType, (int) config->lineNearFarConfig.enableAutoNearFarCheck);
    fprintf( file, "Cycle Difference Threshold (%%), TX, %d, RX, %d, \n", (int) config->lineNearFarConfig.cycleDifferencePercentTx, (int) config->lineNearFarConfig.cycleDifferencePercentRx);
    fprintf( file, "Out of Spec Channel, %d, \n", (int) config->lineNearFarConfig.cycleOutOfChannels);
    fprintf( file, "Cycle Avg Count Number, TX, %d, RX, %d, \n", (int) config->lineNearFarConfig.cycleAvgCountNumberTx, (int) config->lineNearFarConfig.cycleAvgCountNumberRx);
    fprintf( file, "\n" );

    // printf table: CycleTxDiff -------------------------------------------------------
    fprintf(file, "\nCycleTxDiff,\n");
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, CycleTxDiffID); 

    fprintf(file, "\r\n");

    if(chipNum == 1) 
    {
        if((int)config->lineExtraConfig.enableTXExtraCheck == 1)
        {
            fprintf(file, "TX enable,");
        } else 
        {
            fprintf(file, "TX disable,");
        }
    } else 
    {
        if((int)config->lineExtraConfig.enableRXExtraCheck == 1)
        {
            fprintf(file, "TX enable,");
        } else 
        {
            fprintf(file, "TX disable,");
        }
    }

    // printf table: TXPointShortResult ------------------------------------------------
    fprintf(file, "\nTXPointShortResult, PASS 1, FAIL 0,\n");
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, TXPointShortResultID); 

    // printf table: CycleRxDiff -------------------------------------------------------
    fprintf(file, "\nCycleRxDiff,\n");
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, CycleRxDiffID);

    fprintf(file, "\r\n");

    if(chipNum == 1) 
    {
        if((int)config->lineExtraConfig.enableRXExtraCheck == 1)
        {
            fprintf(file, "RX enable,");
        } else 
        {
            fprintf(file, "RX disable,");
        }
    } else 
    {
        if((int)config->lineExtraConfig.enableTXExtraCheck == 1)
        {
            fprintf(file, "RX enable,");
        } else 
        {
            fprintf(file, "RX disable,");
        }
    }

    // printf table: RXPointShortResult ------------------------------------------------
    fprintf(file, "\nRXPointShortResult, PASS 1, FAIL 0,\n");
    MultiOpenShortResult::fprintMultiData(file, multiData, multiResult, config, RXPointShortResultID); 

    // MutualShortExtraCheck, end

    fprintf(file, "\r\n");
    fprintf(file, "\r\n");

    // MutualNearFarCheckByTP, begin
    // TX ------------------------------------------------------------------------------
    fprintf(file, "CheckMutualNearFarTxValue,\n");
    for(int i = 0; i < height; i++)
    {
        fprintf(file, "%5c%3d,", 'Y', i+1);
    }
    fprintf(file, "\n");
    for(int i = 0; i < height; i++)
    {
        fprintf(file, "%8.3f,", multiResult->getYLineTpNearFarValue(i));
    }
    fprintf(file, "\n");
    fprintf(file, "CheckMutualNearFarTxResult, PASS 1, FAIL 0,\n");
    for(int i = 0; i < height; i++)
    {
        switch(multiResult->getYLineTpNearFarResult(i))
        {
        case LINE_PASS:
            fprintf(file, "%8d,", 1);
            break;
        case LINE_NEARFAR_SHORT:
            fprintf(file, "%8d,", 0);
            break;
        default:
            fprintf(file, "%8d,", multiResult->getYLineTpNearFarResult(i));
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
        fprintf(file, "%8.3f,", multiResult->getXLineTpNearFarValue(i));
    }
    fprintf(file, "\n");
    fprintf(file, "CheckMutualNearFarRxResult, PASS 1, FAIL 0,\n");
    for(int i = 0; i < width; i++)
    {
        switch(multiResult->getXLineTpNearFarResult(i))
        {
        case LINE_PASS:
            fprintf(file, "%8d,", 1);
            break;
        case LINE_NEARFAR_SHORT:
            fprintf(file, "%8d,", 0);
            break;
        default:
            fprintf(file, "%8d,", multiResult->getXLineTpNearFarResult(i));
            break;
        }
    }
    // MutualNearFarCheckByTP, end

    return true;        
}
