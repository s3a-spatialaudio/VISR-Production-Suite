#ifndef VISROBJECTVECTOROBSERVER_H_INCLUDED
#define VISROBJECTVECTOROBSERVER_H_INCLUDED

#include "export_symbols.hpp"

class VISRSUPPORT_LIBRARY_SYMBOL VISRObjectVectorObserver
{
public:
    virtual ~VISRObjectVectorObserver(){}
    
    /**
     *  This function is called by the scene controller everytime the observers must be updated
     */
    virtual void update() = 0;
};

#endif  // VISROBJECTVECTOROBSERVER_H_INCLUDED
