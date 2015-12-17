#include "OpenShortData.h"
#include "csv_parser.hpp"
#include <stdlib.h>


OpenShortData::OpenShortData() :
    mWidth(DEFAULT_W)
    ,mHeight(DEFAULT_H)
    ,mTargetVoltage(0)
    ,mBaseVoltage(0)
    ,mRawVoltage(0)
    ,mDiffVoltage(0)
    ,mCycle(0)
    ,mLoop(0)
    ,mReadBaseVoltage(1)
{
    setWH(DEFAULT_W, DEFAULT_H);
}

OpenShortData::OpenShortData(int w, int h) :
    mWidth(w)
    ,mHeight(h)
    ,mTargetVoltage(0)
    ,mBaseVoltage(0)
    ,mRawVoltage(0)
    ,mDiffVoltage(0)
    ,mCycle(0)
    ,mLoop(0)
    ,mReadBaseVoltage(1)
{
    setWH(w, h);
}

void
OpenShortData::setWH(int w, int h)
{

    if(mBaseVoltage != 0)
    {
        delete [] mBaseVoltage;
        delete [] mRawVoltage;
        delete [] mDiffVoltage;
        delete [] mCycle;
        delete [] mLoop;

        mBaseVoltage = 0;
        mRawVoltage = 0;
        mDiffVoltage = 0;
        mCycle = 0;
        mLoop = 0;

    }



    mWidth = w;
    mHeight = h;

    mBaseVoltage = new float[mWidth * mHeight];
    mRawVoltage = new float[mWidth * mHeight];
    mDiffVoltage = new float[mWidth * mHeight];
    mCycle = new float[mWidth * mHeight];
    mLoop = new float[mWidth * mHeight];

    memset(mBaseVoltage, 0, sizeof(float) * mWidth * mHeight);
    memset(mRawVoltage, 0, sizeof(float) * mWidth * mHeight);
    memset(mDiffVoltage, 0, sizeof(float) * mWidth * mHeight);
    memset(mCycle, 0, sizeof(float) * mWidth * mHeight);
    memset(mLoop, 0, sizeof(float) * mWidth * mHeight);


}

void
OpenShortData::setTargetVoltage(float targetVoltage)
{
    mTargetVoltage = targetVoltage;
}

void
OpenShortData::setBaseVoltage(int x, int y, float baseVoltage)
{
    mBaseVoltage[x * mHeight + y] = baseVoltage;
}

void
OpenShortData::setRawVoltage(int x, int y, float rawVoltage)
{
    mRawVoltage[x * mHeight + y] = rawVoltage;
}

void
OpenShortData::setDiffVoltage(int x, int y, float diffVoltage)
{
    mDiffVoltage[x * mHeight + y] = diffVoltage;
}

void
OpenShortData::setCycle(int x, int y, float cycle)
{
    mCycle[x * mHeight + y] = cycle;
}

void
OpenShortData::setLoop(int x, int y, float loop)
{
    mLoop[x * mHeight + y] = loop;
}

int
OpenShortData::getReadData()
{
    return mReadBaseVoltage;
}


void
OpenShortData::setReadData(int baseVoltage)
{
    mReadBaseVoltage = baseVoltage;
}

int
OpenShortData:: getWidth()
{
    return mWidth;
}

int
OpenShortData::getHeight()
{
    return mHeight;
}

float
OpenShortData::getBaseVoltage(int x, int y)
{

    return mBaseVoltage[x * mHeight + y];
}

float
OpenShortData::getTargetVoltage()
{
    return mTargetVoltage;
}

float
OpenShortData::getRawVoltage(int x, int y)
{
    return mRawVoltage[x * mHeight + y];
}

float
OpenShortData::getDiffVoltage(int x, int y)
{
    return mDiffVoltage[x * mHeight + y];
}

float
OpenShortData::getCycle(int x, int y)
{
    return mCycle[x * mHeight + y];
}

float
OpenShortData::getLoop(int x, int y)
{
    return mLoop[x * mHeight + y];
}

bool
OpenShortData::loadSampleSeedResultFile(FILE* file, int &x_inverse, int &y_inverse, bool bFilter)
{
    // bool bRet = true;
    bool bFound;
    int row_count = 0;
    // bool bFormErr;

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
        
        if(nShortNum > 0 && bFilter == false)
            return false;
    }
   
    bFound = false;
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

    if( row_count >= 2 )
        nButton = atoi(row[1].c_str());
    else
        return false;

    if ((nWidth == 0) || (nHeight == 0))
        return false;    

    setWH(nWidth, nHeight);

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
OpenShortData::loadSampleFile(FILE* file)
{
    int row_count = 0;

	int x_inverse = 0;
	int y_inverse = 0;

    const char field_terminator = ',';
    const char line_terminator  = '\n';
    const char enclosure_char   = '"';
	
    int nShortNum = 0; nShortNum = nShortNum;
    int nWidth = 0;
    int nHeight = 0;
    int nButton = 0; nButton = nButton;
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

    setWH(nWidth, nHeight);

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

void
OpenShortData::fprintSampleData(FILE *fp2D, int tableID, int x_inverse, int y_inverse)
{
    char buff[255];

	int width = getWidth();
	int height = getHeight();    

    fprintf(fp2D, "Y\\X,");
    for ( int i = 0; i < width; i++) {

        if(x_inverse == 1) {
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
        if(y_inverse == 1) {
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
            if (x_inverse== 1)
            {
                idxI = width - i - 1;
            } 
            else
            {
                idxI = i;
            }

            switch (tableID) {                
                case VoltageID:
	                sprintf(buff, "%.3f", getBaseVoltage(idxI, idxJ));
                    break;
                case LOOPID:
                    sprintf(buff, "%.0f", getLoop(idxI, idxJ));
                    break;
                case CYCLEID:
                    sprintf(buff, "%.3f", getCycle(idxI, idxJ));
                    break;
                default:
                    break;

            }
            fprintf(fp2D, "%13s,", buff);
        }      
        fprintf(fp2D, "\r\n");
    }
}


bool
OpenShortData::saveSampleFile(FILE* file, int x_inverse, int y_inverse)
{
    if(!file)
    {
        return false;
    }

    fprintf(file, "NG Num,%d,,,\r\n", 0);
	fprintf(file, "X Inverse,%d,Y Inverse,%d,\r\n", x_inverse, y_inverse);
    fprintf(file, "Width,%d,,,\r\n", getWidth());
    fprintf(file, "Height,%d,,,\r\n", getHeight());
    fprintf(file, "Button,%d,,,\r\n", getBtnChannel());

    fprintf(file, "\r\n");

	fprintf(file, "Table%d:,CalibLoopcnt,(Golden Sample)\r\n", LOOPID);
	fprintSampleData(file, LOOPID, x_inverse, y_inverse);

    fprintf(file, "\nTable%d:,CalibSampleTime\r\n", CYCLEID);
    fprintSampleData(file, CYCLEID, x_inverse, y_inverse);

    fprintf(file, "\nTable%d:,Voltage\r\n", VoltageID);
    fprintSampleData(file, VoltageID, x_inverse, y_inverse);
    
    return true;
}

OpenShortData::~OpenShortData()
{

    delete [] mBaseVoltage;
    delete [] mRawVoltage;
    delete [] mDiffVoltage;
    delete [] mCycle;
    delete [] mLoop;

}


bool
OpenShortData::combineData(OpenShortData* resultDatas, int size)
{
    if(size == 0)
    {
        return false;
    }

    int width = resultDatas[0].getWidth();
    int height = resultDatas[0].getHeight();

    setWH(width, height);

    for(int i=0; i<size; i++)
    {
        if(width != resultDatas[i].getWidth() || height != resultDatas[i].getHeight())
        {
            return false;
        }
    }


    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            float baseVoltage = 0;
            float rawVoltage = 0;
            float diffVoltage = 0;
            float cycle = 0;
            float loop = 0;
            for(int k=0; k<size; k++)
            {
                baseVoltage += resultDatas[k].getBaseVoltage(i, j);
                rawVoltage += resultDatas[k].getRawVoltage(i, j);
                diffVoltage += resultDatas[k].getDiffVoltage(i, j);
                cycle += resultDatas[k].getCycle(i, j);
                loop += resultDatas[k].getLoop(i, j);
            }
            setBaseVoltage(i, j, baseVoltage / size);
            setRawVoltage(i, j, rawVoltage / size);
            setDiffVoltage(i, j, diffVoltage / size);
            setCycle(i, j, cycle / size);
            setLoop(i, j, loop / size);
        }
    }

    return true;


}

bool
OpenShortData::combineDataWithFilter(OpenShortData* resultDatas, int size)
{
    if(size == 0)
    {
        return false;
    }

    int width = resultDatas[0].getWidth();
    int height = resultDatas[0].getHeight();

    setWH(width, height);

    for(int i=0; i<size; i++)
    {
        if(width != resultDatas[i].getWidth() || height != resultDatas[i].getHeight())
        {
            return false;
        }
    }


    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            float baseVoltage = 0;
            float rawVoltage = 0;
            float diffVoltage = 0;
            float cycle = 0;
            float loop = 0;
			float minBaseVoltage;
			float maxBaseVoltage;
			float minRawVoltage;
			float maxRawVoltage;
			float minDiffVoltage;
			float maxDiffVoltage;
			float minCycle;
			float maxCycle;
			float minLoop;
			float maxLoop;
			
            for(int k=0; k<size; k++)
            {
	            float tBaseVoltage = resultDatas[k].getBaseVoltage(i, j);
                float tRawVoltage = resultDatas[k].getRawVoltage(i, j);
                float tDiffVoltage = resultDatas[k].getDiffVoltage(i, j);
                float tCycle = resultDatas[k].getCycle(i, j);
                float tLoop = resultDatas[k].getLoop(i, j);
                if (k == 0)
                {
                	minBaseVoltage = tBaseVoltage;
					maxBaseVoltage = tBaseVoltage;
					minRawVoltage = tRawVoltage;
					maxRawVoltage = tRawVoltage;
					minDiffVoltage = tDiffVoltage;
					maxDiffVoltage = tDiffVoltage;
					minCycle = tCycle;
					maxCycle = tCycle;
					minLoop = tLoop;
					maxLoop = tLoop;
                }
				else
				{
					if ( tBaseVoltage < minBaseVoltage )
						minBaseVoltage = tBaseVoltage;
					if ( tBaseVoltage > maxBaseVoltage)
						maxBaseVoltage= tBaseVoltage;
					if ( tRawVoltage < minRawVoltage )
						minRawVoltage = tRawVoltage;
					if ( tRawVoltage > maxRawVoltage )
						maxRawVoltage = tRawVoltage;
					if ( tDiffVoltage < minDiffVoltage )
						minDiffVoltage = tDiffVoltage;
					if ( tDiffVoltage > maxDiffVoltage )
						maxDiffVoltage = tDiffVoltage;
					if ( tCycle < minCycle )
						minCycle = tCycle;
					if ( tCycle > maxCycle )
						maxCycle = tCycle;
					if ( tLoop < minLoop )
						minLoop = tLoop;
					if ( tLoop > maxLoop )
						maxLoop = tLoop;
				}
                baseVoltage += tBaseVoltage;
                rawVoltage += tRawVoltage;
                diffVoltage += tDiffVoltage;
                cycle += tCycle;
                loop += tLoop;
            }
            setBaseVoltage(i, j, (baseVoltage - minBaseVoltage - maxBaseVoltage) / (size - 2));
            setRawVoltage(i, j, (rawVoltage - minRawVoltage - maxRawVoltage) / (size - 2));
            setDiffVoltage(i, j, (diffVoltage - minDiffVoltage - maxDiffVoltage) / (size - 2));
            setCycle(i, j, (cycle - minCycle - maxCycle) / (size - 2));
            setLoop(i, j, (loop - minLoop - maxLoop) / (size - 2));
        }
    }

    return true;


}

void
OpenShortData::writeCalibrationToBuf(char *buf)
{
	int height_align_4 = ( getHeight() + 3 ) / 4 * 4;
	int dataBlockSize = getWidth() * height_align_4;

	for ( int i = 0; i < getWidth(); i++ )
	{
		for ( int j = 0; j < getHeight(); j++ )
		{
			buf[ i * height_align_4 + j ] = (char) (getCycle(i, j) * 40 + 0.5 );

			buf[ i * height_align_4 + j + dataBlockSize ] = getLoop(i, j);
		}
	}
}

bool
OpenShortData::loopCycleToBinFile(FILE *file)
{	
	if(!file)
    {
        return false;
    }

	if ( fseek( file, 0xd000, SEEK_SET ) )
	{
		printf( "File seek to 0xd000 failed\n" );
		return false;
	}

	int height_align_4 = ( getHeight() + 3 ) / 4 * 4;

	unsigned int dataBlockSize = getWidth() * height_align_4;
	
	char* buffer = new char [ dataBlockSize * 2 ];
			
	if ( !buffer )
	{
		printf( "memory allocate failed\n" );
		return false;
	}

	memset( buffer, 0xff, dataBlockSize * 2 );

	writeCalibrationToBuf(buffer);
	
	if ( fwrite( buffer, 1, dataBlockSize * 2, file ) != dataBlockSize * 2 )
	{
		printf( "write Loop/Cycle into file failed\n" );
		delete [] buffer;
		return false;
	}

	if ( fseek( file, 0xeffc, SEEK_SET ) )
	{
		printf( "File seek to 0xeffc failed\n" );
		delete [] buffer;
		return false;
	}

    buffer[0] = 0x50;
    buffer[1] = 0x38;
    buffer[2] = 0x31;
    buffer[3] = 0x30;

    if ( fwrite( buffer, 1, 4, file ) != 4 )
    {
	    printf( "write re-calibration flag into file failed\n" );
		delete [] buffer;
		return false;
    }

    fclose( file );

	delete [] buffer;

	return true;
}
