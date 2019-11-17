#ifndef __RF24FAKEPOOL_H__
#define __RF24FAKEPOOL_H__

#ifdef _WIN32

#include <iostream>
#include <vector>



/*
 Name:		RF24.h
 Created:	08/23/2019 9:04:58 PM
 Author:	Loic Robin
*/

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

// THREAD SAFE
#include <mutex>


class RF24;

class RF24Pool
{
public:
	static RF24Pool* Get()
	{
		static RF24Pool* pool;
		if (!pool)
			pool = new RF24Pool();
		return (pool);
	}

	void AddUnit(RF24* unit)
	{
		while (_mutex.try_lock());
		_units.push_back(unit);
		_mutex.unlock();
	}

	bool Write(const void* buf, uint8_t len);
	void SetFailureRate(float rate) { _failureRate = rate; };

private:

	RF24Pool() {}; // No Constructor
	RF24Pool(const RF24Pool& other) {}; // No Constructor By Copy

	std::vector<RF24*>		_units;
	std::mutex				_mutex;
	float					_failureRate = 0.0f;
};


class RF24PoolInterface
{
public:
	RF24PoolInterface(RF24Pool* pool, RF24* unit)
	{
		std::cout << "RF24PoolInterface Constructor" << std::endl;
		_pool = pool;
		_unit = unit;
		_bufferData = malloc(1); // To avoid if null in set buffer data
		_bufferLength = 1;
		pool->AddUnit(unit);
	}

	bool	WriteInPool(const void* buf, uint8_t len)
	{
		return (_pool->Write(buf, len));
	}

	void	SetBufferData(const void* buf, uint8_t len);

	void* GetBufferData() const {
		return (_bufferData);
	}

	uint8_t GetBufferLength() const {
		return (_bufferLength);
	}

private:

	virtual void OnReceiveData() = 0;

	uint8_t		_bufferLength;
	void*		_bufferData;
	RF24Pool*	_pool;
	RF24*		_unit;
};


#endif // __WIN32__

#endif // __RF24FAKEPOOL_H__