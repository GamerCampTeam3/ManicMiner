// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
//	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:		TSingleton is a template for creating singletons easily, just by inheriting from this				//
//					Initially this was being used as the parent of CLevelManager										//	
//					and the now deprecated CGameInstance																//
//																														//
//					CGameInstance was reworked into the CManicLayer														//
//					and CLevelManager did not need to be a singleton in the first place									//
//					rendering this Template now useless																	//
//																														//
//					Completely commented out and excluded from the build in project settings							//
//					Only exists still for submission purposes															//
// -------------------------------------------------------------------------------------------------------------------- //


//#ifndef _TSINGLETON_H_
//#define _TSINGLETON_H_
//
//template <typename TType>
//class TSingleton
//{
//public:
//	inline static TType* getInstance();
//	inline static void release();
//private:
//	static TType* singleton;
//};
//
//template <typename TType >
//inline TType* TSingleton<TType>::getInstance()
//{
//	if( !singleton )
//	{
//		singleton = new TType;
//	}
//	return singleton;
//}
//
//template<typename TType>
//inline void TSingleton<TType>::release()
//{
//	if( singleton )
//	{
//		delete singleton;
//		singleton = 0;
//	}
//}
//
//template <typename TType>
//TType* TSingleton<TType>::singleton = 0;
//
//#endif // #ifndef _TSINGLE_H_