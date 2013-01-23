#include "csv.h"

#include <QFile>
#define MAXSETLENGTH (1024*1024*10)
#define LINUX 1

csv::csv(QString PathArg)
{
    Path = PathArg;
    delim = ':';
}

void csv::setData(QPolygonF *CurveArg)
{
    Curve = *CurveArg;
}

QPolygonF csv::getData(void)
{
    return Curve;
}

void csv::setDelimiter(char delimA)
{
    delim = delimA;
    Curve.clear();
}

int csv::readCSV(int numRows)
{
    int INFILE;
    char *FileBuf = (char*)malloc(MAXSETLENGTH);
    char *ptr = FileBuf;

    Curve.clear();


    if(ptr == NULL)
        return -1;

    float x,y;

    memset(ptr, 0, MAXSETLENGTH);
#ifdef LINUX
    if((INFILE = open(Path.toStdString().c_str(),O_RDONLY))<=0)
      return -1;
#endif

#ifdef WINDOWS
    if((INFILE = open(Path.toStdString().c_str(), O_RDONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
      return -1;
#endif


    int flength = read(INFILE, FileBuf, MAXSETLENGTH);

    while(ptr < FileBuf + flength && (numRows != 0) )
    {
        char *error;
        numRows --;


        x = strtod(ptr, &error);
        if(error == NULL)
            break;

        while(*ptr != delim && ptr < (FileBuf + flength))
            ptr ++;
        ptr ++;

        if(ptr > (FileBuf + flength))
            break;

        y = strtod(ptr, &error);
        if(error == NULL)
            break;

        while(*ptr != '\n' && ptr < (FileBuf + flength))
            ptr ++;
        ptr ++;
        if(ptr > (FileBuf + flength))
            break;

        Curve.append(QPointF(x, y));
    }

    close(INFILE);

    free(FileBuf);
    return flength;
}


int csv::writeCSV(void)
{
    int LengthOfBuf = Curve.count() * (2*32+4); // 32Bytes per float x,y and " : " "\n
    int OUTFILE;
    int i;
    char *FileBuf = (char*)malloc(LengthOfBuf);
    char *ptr = FileBuf;

    memset(ptr, 0, LengthOfBuf);



    for(i = 0; i < Curve.count() ; i++)
    {
        ptr += sprintf(ptr,"%f : %f\n", Curve.at(i).x(), Curve.at(i).y());
    }
#ifdef LINUX
    if((OUTFILE = open(Path.toStdString().c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
      return -1;
#endif

#ifdef WINDOWS
    QFile OldOne(Path);
    OldOne.remove();
    if((OUTFILE = open(Path.toStdString().c_str(),O_CREAT | O_WRONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
      return -1;
#endif


    int flength = write(OUTFILE, FileBuf, ptr - FileBuf);
    close(OUTFILE);

    free(FileBuf);
    return flength;
}
