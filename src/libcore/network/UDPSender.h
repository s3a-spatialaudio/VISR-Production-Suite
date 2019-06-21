#ifndef UDPSENDER_HEADER
#define UDPSENDER_HEADER

#include "../export_symbols.hpp"
#include "JuceHeader.h"

namespace visr_production_suite
{
namespace core
{
namespace network
{

class CORE_LIBRARY_SYMBOL UDPSender
{
public:
    UDPSender ();
    ~UDPSender();

    void UDPsend();
    bool bind (const juce::String& newTargetHost, int newTargetPort);
    bool unbind();
    bool sendOutputStream ();
    juce::MemoryOutputStream outStream;
    juce::ScopedPointer<juce::DatagramSocket> socketSend;
    juce::String targetHostName;
    int targetPortNumber = 0;
    void UDPsend(std::string message);
    virtual void createMessage() = 0;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UDPSender)
};
    
} // namespace network
} // namespace core
} // namespace visr_production_suite

#endif   // UDPSENDER_HEADER
