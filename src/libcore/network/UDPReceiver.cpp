#include "UDPReceiver.h"

namespace visr_production_suite
{
namespace core
{
namespace network
{

using namespace juce;

UDPReceiver::UDPReceiver (String name):
Thread(name)
{}

UDPReceiver::~UDPReceiver(){}

bool UDPReceiver::bind(int portNumber)
{
    if (! unbind())
        return false;
    
    socketReceive.setOwned (new DatagramSocket (false));
    
    if (! socketReceive->bindToPort (portNumber))
        return false;
    
    startThread();
    return true;
}

bool UDPReceiver::unbind()
{
    if (socketReceive != nullptr)
    {
        signalThreadShouldExit();
        
        if (socketReceive.willDeleteObject())
            socketReceive->shutdown();
        
        waitForThreadToExit (10000);
        socketReceive.reset();
    }
    return true;
}

//exception should be caught here, but it's a different thread..
void UDPReceiver::run()
{
    while (! Thread::threadShouldExit())
    {
        jassert (socketReceive != nullptr);
        char buffer[66536];
        socketReceive->waitUntilReady (true, -1);
        
        if ( Thread::threadShouldExit())
            return;
        
        auto bytesRead = (size_t) socketReceive->read (buffer, (int) sizeof (buffer), false);
        if (bytesRead >= 4)
            handleMessage(buffer, bytesRead);
    }
}

} // namespace network    
} // namespace core
} // namespace visr_production_suite
