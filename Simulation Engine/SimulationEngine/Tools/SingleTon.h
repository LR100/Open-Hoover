#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <stdlib.h>

template <typename T>
class Singleton
{
public:

  // Renvoie L'unique Instance de l'Objet
  static T *GetObj()
  {
    if (_Object == NULL)
        {_Object = new T;}
    return (static_cast<T*> (_Object));
  }
  // Redefinie un nouvel Objet
  static void SetObjet(T* newObject)
  {
      if (newObject == NULL)
        {return;}
      if (_Object != NULL)
        {delete _Object;}
      _Object = newObject;
  }
  // Libere l'unique instance de l'Objet
  static void Kill()
  {
    if (_Object != NULL)
      {
        delete _Object;
        _Object = NULL;
      }
  }
private:
	Singleton() {};
	~Singleton() {};
	Singleton& operator =(const Singleton&);
	Singleton(const Singleton&);
  static T *_Object;
};

template <typename T>
T *Singleton<T>::_Object = NULL;

template <typename S>
class NoNDynSingleton
{
public:
	static S& GetSingleton()
	{
		static S nonDynSingleton;
		return (nonDynSingleton);
	}
protected:
    NoNDynSingleton() {};
    ~NoNDynSingleton() {};
    NoNDynSingleton& operator =(const NoNDynSingleton&);
    NoNDynSingleton(const NoNDynSingleton&);

};

#endif // SINGLETON_H_INCLUDED
