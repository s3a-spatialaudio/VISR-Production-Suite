#ifndef UDPRECEIVER_HEADER
#define UDPRECEIVER_HEADER

#include "../export_symbols.hpp"
#include "JuceHeader.h"

namespace visr_production_suite
{
namespace core
{
namespace network
{

class CORE_LIBRARY_SYMBOL UDPReceiver:
    public juce::Thread
{
public:
    UDPReceiver (juce::String name);
    ~UDPReceiver();
    
    juce::OptionalScopedPointer<juce::DatagramSocket> socketReceive;
    bool bind (int portNumber);
    bool unbind();
    
    virtual void handleMessage(const char* data, size_t dataSize) = 0;
    
    //it's the UDP receive loop.
    void run() override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UDPReceiver)
};

} // namespace network
} // namespace core
} // namespace visr_production_suite

#endif   // UDPRECEIVER_HEADER
