#include "MultiOpenShortData.h"
#include "csv_parser.hpp"

MultiOpenShortData::MultiOpenShortData() :
    mChipNum(1)
    ,mDatas(0)
    ,mChipOrderIndexs(0)
    ,mReadBaseVoltage(1)	
{
    setChipNum(1);
}

MultiOpenShortData::MultiOpenShortData(int chipNum) :
    mChipNum(chipNum)
    ,mDatas(0)
    ,mChipOrderIndexs(0)
    ,mReadBaseVoltage(1)	
{
    if(chipNum < 1)
    {
        printf("minimum chipNum is 1.\n");
        chipNum = 1;
    }

	setChipNum(chipNum);
}

MultiOpenShortData::~MultiOpenShortData()
{
    delete [] mDatas;
    delete [] mChipOrderIndexs;
}

void
MultiOpenShortData::setChipNum(int chipNum)
{
    if(mDatas != 0)
    {
        delete [] mDatas;
        delete [] mChipOrderIndexs;
    }

    mChipNum = chipNum;
    mDatas = new OpenShortData[chipNum];
    mChipOrderIndexs = new int[chipNum];

    for(int i=0;i<chipNum;i++)
    {
        mChipOrderIndexs[i] = i;
    }
}

int
MultiOpenShortData::getChipNum()
{
    return mChipNum;
}

bool
MultiOpenShortData::setOrderInfo(OpenShortConfig* config)
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
MultiOpenShortData::setChipOrderIndex(int index, int orderIndex)
{
    mChipOrderIndexs[index] = orderIndex;
}

int
MultiOpenShortData::getChipOrderIndex(int index)
{
    return mChipOrderIndexs[index];
}

OpenShortData*
MultiOpenShortData::getData(int index)
{
    if(index < mChipNum)
    {
        return &(mDatas[index]);
    }
    else
    {
        return NULL;
    }
}

void
MultiOpenShortData::queryMappingSingle(int &devIdx, int &x, int &y)
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
MultiOpenShortData::getWidth()
{
    int width = 0;
    if(mChipNum == 1)
    {
        width += mDatas[0].getWidth();
    }
    else
    {
        for(int index = 0; index < mChipNum; index++)
        {
            width += mDatas[index].getHeight();
        }
    }
    return width;
}

int
MultiOpenShortData::getWidth(int index)
{
    if(index < mChipNum && index >= 0)
    {
        return mDatas[index].getWidth();
    }
    else
    {
        return 0;
    }

}

int
MultiOpenShortData::getHeight()
{
    int height = 0;
    if(mChipNum == 1)
    {
        height = mDatas[0].getHeight();
    }
    else if(mChipNum < 1)
    {
        height = 0;
    }
    else
    {
        //height =  mDatas[0].getWidth();
    	for(int i = 0; i < mChipNum; i++)
    	{
    		if(mDatas[i].getWidth() > height) // get max
    		{
    			height = mDatas[i].getWidth();
    		}
    	}
    }
    return height;
}

int
MultiOpenShortData::getHeight(int index)
{
    if(index < mChipNum && index >= 0)
    {
        return mDatas[index].getHeight();
    }
    else
    {
        return 0;
    }
}

int
MultiOpenShortData::getBtnChannel()
{
    return 0;
}

float
MultiOpenShortData::getCycle(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getCycle(x, y);
}

float
MultiOpenShortData::getLoop(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getLoop(x, y);
}

float
MultiOpenShortData::getBaseVoltage(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getBaseVoltage(x, y);
}

float
MultiOpenShortData::getRawVoltage(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getRawVoltage(x, y);
}

float
MultiOpenShortData::getDiffVoltage(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getDiffVoltage(x, y);
}

float
MultiOpenShortData::getTargetVoltage(int x, int y)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    return mDatas[devIdx].getTargetVoltage();
}


void
MultiOpenShortData::setCycle(int x, int y, float cycle)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setCycle(x, y, cycle);
}

void
MultiOpenShortData::setLoop(int x, int y, float loop)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setLoop(x, y, loop);
}

void
MultiOpenShortData::setBaseVoltage(int x, int y, float baseVoltage)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setBaseVoltage(x, y, baseVoltage);
}

void
MultiOpenShortData::setRawVoltage(int x, int y, float rawVoltage)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setRawVoltage(x, y, rawVoltage);
}

void
MultiOpenShortData::setDiffVoltage(int x, int y, float diffVoltage)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setDiffVoltage(x, y, diffVoltage);
}

void
MultiOpenShortData::setTargetVoltage(int x, int y, float targetVoltage)
{
    int devIdx = 0;
    queryMappingSingle(devIdx, x, y);
    mDatas[devIdx].setTargetVoltage(targetVoltage);
}

int
MultiOpenShortData::getReadData()
{
    return mReadBaseVoltage;
}

void
MultiOpenShortData::setReadData(int baseVoltage)
{
    mReadBaseVoltage = baseVoltage;
}

bool
MultiOpenShortData::saveSampleFile(FILE* file)
{

    if(mChipNum < 1)
    {
        return false;
    }
    else
    {
        if(!file)
        {
            return false;
        }

        fprintf(file, "NG Num,%d,,,\r\n", 0);
        fprintf(file, "Width,%d,,,\r\n", getWidth());
        fprintf(file, "Height,%d,,,\r\n", getHeight());
        fprintf(file, "Button,%d,,,\r\n", getBtnChannel());

        fprintf(file, "\r\n");
        fprintf(file, "   Node,   X-ray, Y-ray, CalibLoopcnt, CalibSampleTime, MutualVoltage\r\n");

        for ( int i = 0; i < getWidth(); i ++)
        {
            for ( int j = 0; j < getHeight(); j ++)
            {

                fprintf(file, "%7d, %7d, %5d, %12.3f, %13.3f, %12.3f\r\n", (i*getHeight())+(j+1),(i+1),(j+1),
                        getLoop(i, j), getCycle(i, j), getBaseVoltage(i, j));
            }
        }

        fprintf(file, "\r\n");
        fprintf(file, "   Node,  CalibLoopcnt, CalibSampleTime, MutualVoltage\r\n");

        return true;
    }
    
}

void
MultiOpenShortData::divideToLike(MultiOpenShortData* likeObject)
{
    // int oldChipNum = mChipNum;
    int oldWidth = getWidth();
    int oldHeight = getHeight();
    float ** oldTargetVoltage;
    float ** oldBaseVoltage;
    float ** oldRawVoltage;
    float ** oldDiffVoltage;
    float ** oldCycle;
    float ** oldLoop;

    oldTargetVoltage =  new float*[oldWidth];
    oldBaseVoltage = new float*[oldWidth];
    oldRawVoltage = new float*[oldWidth];
    oldDiffVoltage = new float*[oldWidth];
    oldCycle = new float*[oldWidth];
    oldLoop = new float*[oldWidth];


    for(int i=0; i<oldWidth; i++)
    {
        oldTargetVoltage[i] = new float[oldHeight];
        oldBaseVoltage[i] = new float[oldHeight];
        oldRawVoltage[i] = new float[oldHeight];
        oldDiffVoltage[i] = new float[oldHeight];
        oldCycle[i] = new float[oldHeight];
        oldLoop[i] = new float[oldHeight];

        for(int j=0; j<oldHeight; j++)
        {
            oldTargetVoltage[i][j] = getTargetVoltage(i, j);
            oldBaseVoltage[i][j] = getBaseVoltage(i, j);
            oldRawVoltage[i][j] = getRawVoltage(i, j);
            oldDiffVoltage[i][j] = getDiffVoltage(i, j);
            oldCycle[i][j] = getCycle(i, j);
            oldLoop[i][j] = getLoop(i, j);			
        }
    }    

    int newChipNum = likeObject->getChipNum();
    setChipNum(newChipNum);

    for(int i=0; i<getChipNum(); i++)
    {        
        getData(i)->setWH(likeObject->getData(i)->getWidth(), likeObject->getData(i)->getHeight());
        setChipOrderIndex(i, likeObject->getChipOrderIndex(i));
    }

    for(int i=0; i<oldWidth; i++)
    {
        for(int j=0; j<oldHeight; j++)
        {
            setTargetVoltage(i, j, oldTargetVoltage[i][j]);
            setBaseVoltage(i, j, oldBaseVoltage[i][j]);
            setRawVoltage(i, j, oldRawVoltage[i][j]);
            setDiffVoltage(i, j, oldDiffVoltage[i][j]);
            setCycle(i, j, oldCycle[i][j]);
            setLoop(i, j, oldLoop[i][j]);
        }
    }

    for(int i=0; i<oldWidth; i++)
    {
        delete[] oldTargetVoltage[i];
        delete[] oldBaseVoltage[i];
        delete[] oldRawVoltage[i];
        delete[] oldDiffVoltage[i];
        delete[] oldCycle[i];
        delete[] oldLoop[i];
    }

    delete[] oldTargetVoltage;
    delete[] oldBaseVoltage;
    delete[] oldRawVoltage;
    delete[] oldDiffVoltage;
    delete[] oldCycle;
    delete[] oldLoop;    


}

bool
MultiOpenShortData::loadSampleFile(FILE* file)
{
    int row_count = 0;

    const char field_terminator = ',';
    const char line_terminator  = '\n';
    const char enclosure_char   = '"';

	int x_inverse = 0;
	int y_inverse = 0;
	
    int nShortNum = 0;
    nShortNum = nShortNum;
    int nWidth = 0;
    int nHeight = 0;
    int nButton = 0;
    nButton = nButton;
    csv_parser file_parser;

    if(!file_parser.init(file))
    {
        return false;
    }

    /* Here we tell the parser how to parse the file */
    file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);
    file_parser.set_field_term_char(field_terminator);
    file_parser.set_line_term_char(line_terminator);

    csv_row row = file_parser.get_row();
    row_count = row.size();
    

    if ( row_count >= 2 )
        nShortNum = atoi(row[1].c_str());
    else
        return false;

    //Find Width
    bool bFound = false;

    do
    {
        if(file_parser.has_more_rows())
            row = file_parser.get_row();
        else
            break;

        row_count = row.size();
        if((row_count >= 2) && (strcmp(row[0].c_str(),"Width") == 0))
        {
            bFound = true;
            break;
        }
		else if((row_count >= 4) && (strcmp(row[0].c_str(),"X Inverse") == 0))
        {
            x_inverse = atoi(row[1].c_str());
			y_inverse = atoi(row[3].c_str());
        }
    } while(1);
	
	if(bFound == false)
		return false;
    
    nWidth = atoi(row[1].c_str());

    row = file_parser.get_row();
    row_count = row.size();
    if( row_count >= 2 )
        nHeight = atoi(row[1].c_str());
    else
        return false;

    row = file_parser.get_row();
    row_count = row.size();
    if( row_count >= 1)
        nButton = atoi(row[1].c_str());
    else
        return false;

    if ((nWidth == 0) || (nHeight == 0))
        return false;

    setChipNum(1);
    mDatas[0].setWH(nWidth, nHeight);

	int idxI, idxJ;

    //Skip Empty Row
    file_parser.get_row();
    //Skip title
    file_parser.get_row();
	//Skip X index
	file_parser.get_row();
	
	for(int j = 0; j < nHeight ; j++)
	{
		row = file_parser.get_row();	

		row_count = row.size();
		if (row_count < nWidth + 2 )
			return false;

		if( y_inverse == 1 )				 
			idxJ = j;
		else
			idxJ = nHeight - j - 1;

		for (int i = 0; i < nWidth; i++)
		{
			if( x_inverse == 1 )			 
				idxI = nWidth - i - 1;
			else
				idxI = i;

			setLoop(idxI, idxJ, (float) atof(row[i+1].c_str()));
		}
	}


	//Skip Empty Row
	file_parser.get_row();
	//Skip title
	file_parser.get_row();
	//Skip X index
	file_parser.get_row();
	
	for(int j = 0; j < nHeight ; j++)
	{
		row = file_parser.get_row();	

		row_count = row.size();
		if (row_count < nWidth + 2 )
			return false;

		if( y_inverse == 1 )				 
			idxJ = j;
		else
			idxJ = nHeight - j - 1;

		for (int i = 0; i < nWidth; i++)
		{
			if( x_inverse == 1 )			 
				idxI = nWidth - i - 1;
			else
				idxI = i;

			setCycle(idxI, idxJ, (float) atof(row[i+1].c_str()));
		}
	}


	//Skip Empty Row
	file_parser.get_row();
	//Skip title
	file_parser.get_row();
	//Skip X index
	file_parser.get_row();
	
	for(int j = 0; j < nHeight ; j++)
	{
		row = file_parser.get_row();	

		row_count = row.size();
		if (row_count < nWidth + 2 )
			return false;

		if( y_inverse == 1 )				 
			idxJ = j;
		else
			idxJ = nHeight - j - 1;

		for (int i = 0; i < nWidth; i++)
		{
			if( x_inverse == 1 )			 
				idxI = nWidth - i - 1;
			else
				idxI = i;

			setBaseVoltage(idxI, idxJ, (float) atof(row[i+1].c_str()));		
		}
	}
    
    return true;
}

bool
MultiOpenShortData::loadSampleSeedResultFile(FILE* file)
{
    // bool bRet = true;
    bool bFound;
    int row_count = 0;
    bool bFormErr;

    const char field_terminator = ',';
    const char line_terminator  = '\n';
    const char enclosure_char   = '"';
    int nShortNum = 0;
    int nWidth = 0;
    int nHeight = 0;
    int nButton = 0;
    nButton = nButton;
    csv_parser file_parser;

    file_parser.init(file);

    /* Here we tell the parser how to parse the file */
    file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);
    file_parser.set_field_term_char(field_terminator);
    file_parser.set_line_term_char(line_terminator);

    bFound = false;
    csv_row row = file_parser.get_row();
    row_count = row.size();
    if((row_count >= 2) && (strcmp(row[0].c_str(),"NG Num") == 0 || strcmp(row[0].c_str(),"Short Num") == 0))
    {
        bFound = true;
        nShortNum = atoi(row[1].c_str());
        
        if(nShortNum > 0)
            return false;
    }
    else
    {
        do
        {
            if(file_parser.has_more_rows())
                row = file_parser.get_row();
            else
                break;

            row_count = row.size();
            if((row_count >= 2) && (strcmp(row[0].c_str(),"NG Num") == 0))
            {
                bFound = true;
                nShortNum = atoi(row[1].c_str());
                if(nShortNum > 0)
                    return false;
                break;
            }
        }
        while((row_count < 2) || (strcmp(row[0].c_str(),"NG Num") != 0));

        if(bFound == false)
        {
            do
            {
                if(file_parser.has_more_rows())
                    row = file_parser.get_row();
                else
                    break;

                row_count = row.size();
                if((row_count >= 2) && (strcmp(row[0].c_str(),"Short Num") == 0 ))
                {
                    bFound = true;
                    nShortNum = atoi(row[1].c_str());
                    if(nShortNum > 0)
                        return false;
                    break;
                }
            }
            while((row_count < 2) || (strcmp(row[0].c_str(),"Short Num") != 0));
        }
    }
    if(bFound == false)
        return false;

    bFound = false;
    row = file_parser.get_row();
    row_count = row.size();
    if((row_count >= 2) && (strcmp(row[0].c_str(),"Width") == 0))
    {
        bFound = true;
        nWidth = atoi(row[1].c_str());
        if(nWidth == 0)
            return false;
    }
    else
    {
        do
        {
            if(file_parser.has_more_rows())
                row = file_parser.get_row();
            else
                break;

            row_count = row.size();
            if((row_count >= 2) && (strcmp(row[0].c_str(),"Width") == 0))
            {
                bFound = true;
                nWidth = atoi(row[1].c_str());
                if(nWidth == 0)
                    return false;
                break;
            }
        }
        while((row_count < 2) || (strcmp(row[0].c_str(),"Width") != 0));
    }
    if(bFound == false)
        return false;


    bFound = false;
    row = file_parser.get_row();
    row_count = row.size();
    if((row_count >= 2) && (strcmp(row[0].c_str(),"Height") == 0))
    {
        bFound = true;
        nHeight = atoi(row[1].c_str());
        if(nHeight == 0)
            return false;
    }
    else
    {
        do
        {
            if(file_parser.has_more_rows())
                row = file_parser.get_row();
            else
                break;

            row_count = row.size();
            if((row_count >= 2) && (strcmp(row[0].c_str(),"Height") == 0))
            {
                bFound = true;
                nHeight = atoi(row[1].c_str());
                if(nHeight == 0)
                    return false;
                break;
            }
        }
        while((row_count < 2) || (strcmp(row[0].c_str(),"Height") != 0));
    }
    if(bFound == false)
        return false;


    bFound = false;
    row = file_parser.get_row();
    row_count = row.size();
    if((row_count >= 2) && (strcmp(row[0].c_str(),"Button") == 0))
    {
        bFound = true;
        nButton = atoi(row[1].c_str());
    }
    else
    {
        do
        {
            if(file_parser.has_more_rows())
                row = file_parser.get_row();
            else
                break;

            if((row_count >= 2) && (strcmp(row[0].c_str(),"Button") == 0))
            {
                bFound = true;
                nButton = atoi(row[1].c_str());
                break;
            }
        }
        while((row_count < 2) || (strcmp(row[0].c_str(),"Button") != 0));
    }
    if(bFound == false)
        nButton = 0;

    if ((nWidth == 0) || (nHeight == 0))
        return false;

    setChipNum(1);
    mDatas[0].setWH(nWidth, nHeight);

    //Skip Empty Row
    row = file_parser.get_row();
    //Skip title
    row = file_parser.get_row();

    row_count = row.size();

    bFormErr = false;
    if( row_count == 9)
    {
        bFormErr = false;
    }
    else
    {
        bFormErr = true;
    }

    if(bFormErr == true)
        return false;

    int index = 0;


    /*bFormErr = false;*/
    for(int i = 0 ; i < nWidth ; i++)
    {
        for(int j = 0 ; j < nHeight ; j++)
        {

            /* Get the record */
            csv_row row = file_parser.get_row();

            row_count = row.size();
            if ( row_count < 6 )
                break;
            //if( row_count == 6  || row_count == 8 || row_count == 9  || row_count == 10  || row_count == 11  )
            //{
            //	bFormErr = false;
            //}
            //else
            //{
            //	bFormErr = true;
            //	break;
            //}
            setLoop(i, j, (float) atof(row[3].c_str()));
            setCycle(i, j, (float) atof(row[4].c_str()));
            setBaseVoltage(i, j, (float) atof(row[5].c_str()));

            index ++;
        }
    }

    //if(bFormErr == true)
    //	return false;

    if( index != nWidth * nHeight)
        return false;

    return true;
}
