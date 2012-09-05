/******************************
Prime Generation Example Plugin.
===============================

This plugin takes as an input two inclusive ranges of numbers.
These are used to generate a list of prime numbers, then send them back
in a response.

*******************************/

#include "ColdSdk/ColdPluginExport.h"
#include "common/jobtypes.h"

#include <unistd.h>

#define SIMULATE_DELAY 1

#include <stdio.h>

#define IsSet(bit, buffer) (buffer[(bit >> 3)] >> (bit & 0x07) & 0x01)
#define OnBit(bit, buffer) (buffer[(bit >> 3)] |= (1 << (bit & 0x07)))
#define OffBit(bit, buffer) (buffer[(bit >> 3)] &= ~(1 << (bit & 0x07)))

//Functions that help computing prime numbers
void setAllBitsOn(char * b, int sz)
{
    for (int i = 0; i < sz; ++i)
    {
        b[i] = 0xFF;
    }
}

void generatePrimes(char * buffer, int bufferSize, int maxNumber)
{
    setAllBitsOn(buffer, bufferSize); 
    int lastprime = 2;
    while (lastprime < maxNumber)
    {
        for (int i = lastprime * 2; i <= maxNumber; i += lastprime)
        {
            OffBit(i - 1, buffer); 
        }
        
        while (lastprime <= maxNumber)
        {
            lastprime++;
            if (IsSet(lastprime - 1, buffer))
            {
                break;
            }
        }

    }    
}
//END//

class PrimePlugin : public ColdSdk::ColdPlugin
{
public:
    PrimePlugin() 
    {
    }
    virtual ~PrimePlugin()
    {
    }
    virtual int PluginMain(int inputBufferSize, const char * inputBuffer, int outputBufferSize, char * outputBuffer)
    {
        const ComputePrimesJob * job = reinterpret_cast<const ComputePrimesJob*>(inputBuffer);
        ComputedPrimesResponse * response = reinterpret_cast<ComputedPrimesResponse*>(outputBuffer);
        
        response->count = 0;

        int inputSize = job->endNumber - job->initialNumber + 1;
        if (inputSize <= 0)
        {
            return 0;
        }

        int initialNumber = job->initialNumber > 0 ? job->initialNumber : 1;  
        int endNumber = inputSize > MAX_PRIME_NUMBERS ? job->initialNumber + MAX_PRIME_NUMBERS - 1 : job->endNumber; 

        int size = endNumber - initialNumber + 1;

        int numberOfBytes = (endNumber + 1) / 8 + (endNumber % 8 ? 1 : 0); 
        char * buffer = new char[numberOfBytes];
        generatePrimes(buffer, numberOfBytes, endNumber);

        for (int i = 0, n = 0; i < size; ++i)
        {
            int index = i + initialNumber - 1;
            if (IsSet(index, buffer))
            {
                response->primeList[n++] = index + 1;
                ++response->count;
            }
        }
        delete buffer;

#if SIMULATE_DELAY
        sleep(6);
#endif
        return 0;
    }
};


ColdSdk::ColdPlugin * ColdSdk::ExportColdPlugin()
{
    return new PrimePlugin();
}

