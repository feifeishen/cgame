#include<stdio.h>
#include<stdlib.h>

const int BUFFER_SIZE = 1000 ;
const int MAX_VALUE = BUFFER_SIZE * 8;
const int BYTE_WIDTH = 8;

typedef struct BIT_MAP
{
    char *buffer;
    int size;
    long MAX_VALUE;
}BITMAP , *PBMAP;
const int BYTE_ARRY[] = {1<<0,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7};

void * get_char_buffer(int size);
int * getbyte(char * buffer , const int buffer_size, const int size);
void setbyte(char * buffer , const unsigned int num);
PBMAP createBitMap(const int size);
bool setBitMap(PBMAP bitMap , const unsigned int num);
bool setbuffer(void * buffer ,const  int buffersize, const char num);
int *getByteMap(PBMAP bitMap , const unsignedint size);
PBMAP addBitMap(PBMAP bitMap1, PBMAP bitMap2);
int existNum(PBMAP bitMap , int num);







int main(void)
{
    PBMAP bitmap = createBitMap(1000);
    setBitMap(bitmap , 7);
    setBitMap(bitmap,  2);
    setBitMap(bitmap , 101);
    setBitMap(bitmap , 1);
    setBitMap(bitmap , 5);
    setBitMap(bitmap , 1001);
    setBitMap(bitmap , 1234);
    setBitMap(bitmap , 9000);
    setBitMap(bitmap, 1024);
    printf("%d\n" , existNum(bitmap , 90000));
    printf("%d\n" , existNum(bitmap , 1234));
    printf("%d\n" , existNum(bitmap , 8));
    int * arry  = getByteMap(bitmap, 10);
    for(int i = 0 ; i < 9 ; i++)
    {
        printf("%d \n" , *(arry+i));
    }
    return 0;
}

PBMAP createBitMap(const int size)
{
    PBMAP bitMap = (PBMAP)get_char_buffer(sizeof(BITMAP));
    if(bitMap != NULL)
    {
        bitMap->buffer = (char *)get_char_buffer(size);
        if(bitMap->buffer == NULL)
        {
            return NULL;
        }
        setbuffer((void *)bitMap->buffer , size , 0);
        bitMap->size = size;
        bitMap->MAX_VALUE =size * BYTE_WIDTH;
    }
    return bitMap;
}


bool setBitMap(PBMAP bitMap ,const int num)
{
    if(bitMap != NULL && (*bitMap).MAX_VALUE >= num)
    {
        bitMap->buffer[num / 8] = bitMap->buffer[num / 8] +  BYTE_ARRY[num % 8];
        return true;
    }
    return false;
}


int *getByteMap(PBMAP bitMap , const int size)
{

    int * arry = (int *)get_char_buffer(size * 4);
    int index = bitMap->size - 1;
    int arryIndex = 0;
    int byteIndex = 0;
    for( ;index >=0 ; index-- )
    {
        byteIndex = 7;
        for(;byteIndex >= 0 ; byteIndex--)
        {
            if(bitMap->buffer[index] & BYTE_ARRY[byteIndex])
            {
                *(arry + arryIndex) = index * 8 + byteIndex;
                arryIndex = arryIndex + 1;
                if(arryIndex == size)
                {
                    return arry ;
                }
            }
        }
    }
    return arry;
}

int * getbyte(char * buffer , int buffer_size, const int size)
{

    int * arry = (int *)get_char_buffer(size * 4);
    int index = buffer_size - 1;
    int byteIndex;
    int arryIndex = 0;
    setbuffer((void *)arry  , size * 4 , 0);
    for(;index >= 0 ; index--)
    {
        byteIndex = 7;
        for( ; byteIndex >= 0 ; byteIndex --)
        {
            if(buffer[index] & BYTE_ARRY[byteIndex])
            {
                *(arry + arryIndex) = index * 8 + byteIndex;
                arryIndex = arryIndex + 1;
                if(arryIndex == size)
                {
                    return arry ;
                }
            }
        }
    }
    return arry;
}


void setbyte(char * buffer , const unsigned int num)
{
    if(num > MAX_VALUE)
    {
        printf("num must be < %d" , MAX_VALUE);
        return;
    }
    int high = num / 8 ;
    int low = num % 8;
    *(buffer + high) = *(buffer + high) +  BYTE_ARRY[low] ;
}


void * get_char_buffer(int size)
{
    return malloc(size);
}


bool setbuffer(void * buffer ,const int buffersize, const char num)
{
    int index = 0;
    char *pChar = (char *)buffer;
    if(pChar != NULL)
    {
        for( ; index < buffersize ; index++)
        {
            pChar[index] = num;
        }
        return true;
    }
    return false;
}


int existNum(PBMAP bitMap , unsigned int num)
{
    if(bitMap!= NULL && bitMap->MAX_VALUE >= num)
    {
        return (bitMap->buffer[num / BYTE_WIDTH] & BYTE_ARRY[num % BYTE_WIDTH])> 0 ? 1 : 0 ;
    }
    return -1;
}

unsigned int SDBMHash(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
unsigned int RSHash(char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
unsigned int JSHash(char *str)
{
    unsigned int hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
unsigned int PJWHash(char *str)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    while (*str)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash(char *str)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;

    while (*str)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// AP Hash Function
unsigned int APHash(char *str)
{
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }
    return (hash & 0x7FFFFFFF);
}
