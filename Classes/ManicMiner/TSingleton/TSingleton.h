

#ifndef _TSINGLETON_H_
#define _TSINGLETON_H_

template <typename TType>
class TSingleton
{
public:
	inline static TType* getInstance();
	inline static void release();
private:
	static TType* singleton;
};

template <typename TType >
inline TType* TSingleton<TType>::getInstance()
{
	if( !singleton )
	{
		singleton = new TType;
	}
	return singleton;
}

template<typename TType>
inline void TSingleton<TType>::release()
{
	if( singleton )
	{
		delete singleton;
		singleton = 0;
	}
}

template <typename TType>
TType* TSingleton<TType>::singleton = 0;

#endif // #ifndef _TSINGLE_H_