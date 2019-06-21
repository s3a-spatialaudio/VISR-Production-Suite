#include "UDPSender.h"

namespace visr_production_suite
{
namespace core
{
namespace network
{

using namespace juce;

UDPSender::UDPSender (){}

UDPSender::~UDPSender(){}


bool UDPSender::bind (const String& newTargetHost, int newTargetPort)
{
    if (! unbind())
        return false;
    
    socketSend = new DatagramSocket (true);
    targetHostName = newTargetHost;
    targetPortNumber = newTargetPort;
    
    if (socketSend->bindToPort (0)) // 0 = use any local port assigned by the OS.
        //        std::cout<<"current bound port " << socket->getBoundPort () <<std::endl;
        return true;
    
    socketSend = nullptr;
    return false;
}

bool UDPSender::unbind()
{
    socketSend = nullptr;
    return true;
}

bool UDPSender::sendOutputStream()
{
    if (socketSend != nullptr)
    {
        const int streamSize = (int) outStream.getDataSize();
        
        const int bytesWritten = socketSend->write (targetHostName, targetPortNumber,
                                                    outStream.getData(), streamSize);
        return bytesWritten == streamSize;
    }
    
    // if you hit this, you tried to send some OSC data without being
    // connected to a port! You should call OSCSender::connect() first.
    jassertfalse;
    
    return false;
}

void UDPSender::UDPsend(std::string message){
    outStream.write( message.c_str(), message.length() );
    //    outStream.write( jsonMsg.getCharPointer(), jsonMsg.length() );
    sendOutputStream ();
    outStream.reset();
}

} // namespace network
} // namespace core
} // namespace visr_production_suite
