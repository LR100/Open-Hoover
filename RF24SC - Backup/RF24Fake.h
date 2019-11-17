#ifndef __RF24FAKE_H__
#define __RF24FAKE_H__


#ifdef _WIN32

#include <iostream>

/*
 Name:		RF24.h
 Created:	08/23/2019 9:04:58 PM
 Author:	Loic Robin
*/

//////////////////////////////////
//////////////////////////////////
//
//  SerialArduino
//
//	simulate usage of Serial << "msg";
//	link on Arduino
//	thread - safe
//
//////////////////////////////////
//////////////////////////////////


// Serial Thread safe
#include <ostream>
#include <mutex>
#include <unordered_map> // QUICK FIX
#include <sstream> // QUICK FIX

class SerialArduino
{
public:
	
	SerialArduino() {};

	template <typename T>
	SerialArduino& operator<<(const T& a) {
		static std::mutex mutexSerial;
		
		uint64_t			id = std::hash<std::thread::id>{}(std::this_thread::get_id());
		std::stringstream ss;
		ss << a;
		std::string message;
		message = ss.str();
		_buffers[id] += message;
		size_t found = message.find_last_of('\n');


		if (found != std::string::npos && found == (message.size() - 1)) // At end of message
		{
			for (int i = 0; i < 200; i += 1)
			{
				if (mutexSerial.try_lock())
				{
					std::cout << _buffers.at(id);
					_buffers.at(id) = "";
					mutexSerial.unlock();
					i = 4096;
				}
			}
		}
		return (*this);
	}


private:
	std::unordered_map<uint64_t, std::string>	_buffers;
};

extern SerialArduino Serial;


/////////////////////////////////////////////////////
//												// //
//   			!!!!!!!!!!!!!!!!!!  			// //
//												// //
//					class RF24					// //	
//												// //
//			!!  THIS CLASS IF FAKE	 !!  		// //
//												// //
//			  Use Only to debug RF24SC			// //
//												// //
//   			!!!!!!!!!!!!!!!!!!  			// //
//												// //
/////////////////////////////////////////////////////

#include "RF24FakePool.h"


//void shared_cout(std::string msg)
//{
//	static std::mutex mu;
//	mu.lock();
//	std::cout << msg << std::endl;
//	mu.unlock();
//}

class RF24 : public RF24PoolInterface
{
public:

	RF24(uint8_t pince, uint8_t pincs) : RF24PoolInterface(RF24Pool::Get(), this)
	{
		_available = false;
		_listening = false;
	}

	virtual ~RF24()
	{
	}

	void printDetails()
	{

	}

	bool write(const void* buf, uint8_t len)
	{
		if (!_listening)
			return (WriteInPool(buf, len));
		return (false);
	}

	void startListening()
	{
		_listening = true;
	}

	void stopListening()
	{
		_listening = false;
	}

	bool begin()
	{
		return (true);
	}

	void setRetries(uint8_t delay, uint8_t count)
	{

	}

	void enableDynamicPayloads()
	{
		// Do nothing
	}

	void powerDown()
	{
		// Module is running Slower - Do Nothing
	}

	void powerUp()
	{
		// Module is running at Full Speed
	}

	bool available(uint8_t* pipe_num)
	{
		if (!_listening)
			return (false);
		// Return true if package is available for this pipe
		//Serial << "Availabe\n";
		return (_available);
	}

	uint8_t	getDynamicPayloadSize()
	{
		return (RF24PoolInterface::GetBufferLength());
	}

	void read(void* buf, uint8_t len)
	{
		memcpy(buf, RF24PoolInterface::GetBufferData(), len);
		_available = false;
	}

private:

	virtual void OnReceiveData() override
	{
		_available = true;
	}

	bool		_listening;
	bool		_available;
};

#endif // __WIN32__



#endif // __RF24FAKE_H__