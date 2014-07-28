#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include "../AGPacket.h"

//Interface for the persistance unit.
//Any class that implements this can be used for persistence
class DBInterface
{
    public:
		virtual void writeDetection(AGPacket info) = 0;
};

#endif // DBINTERFACE_H
