#ifndef DATASTREAMLISTENER_H
#define DATASTREAMLISTENER_H

/*
 * This is an interface to implement the Action/Listener design pattern
 * The interface action method (called updateData)  is called whenever a
 * new block of data are received
 */
class DataStreamListener{
public:
    DataStreamListener(){}
    virtual ~DataStreamListener(){}
    virtual void updateData(int blockNumber)=0; //pure virtual method
    virtual void setId(int id)=0;
    virtual int getId()=0;
};

#endif // DATASTREAMLISTENER_H
