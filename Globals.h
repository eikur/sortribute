#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MemLeaks.h"
#include "3rdparty/SDL/include/SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

// Useful typedefs ---------
typedef unsigned int uint;

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }
// Defines a unique ptr of a class member, and its getter
#define MEMBER_DECL( _CLASS_, _NAME_, _GETTER_ )	\
		private:								\
			std::unique_ptr<_CLASS_> _NAME_;	\
		public:									\
			_CLASS_& _GETTER_() const	\
			{							\
				return *_NAME_.get();	\
			}

// Defines a member, its const getter and its getter
#define MEMBER_CONST_GET_SET( _CLASS_, _NAME_, _GETTER_, _SETTER_ )	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			const _CLASS_& _GETTER_() const	\
			{							\
				return _NAME_;	\
			} \
			void _SETTER_(int value) \
			{ \
				_NAME_ = value; \
			}

// Defines a member, its const getter 
#define MEMBER_CONST_GET( _CLASS_, _NAME_, _GETTER_)	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			const _CLASS_& _GETTER_() const	\
			{							\
				return _NAME_;	\
			} 

// Defines a member, its const getter 
#define MEMBER_GET( _CLASS_, _NAME_, _GETTER_)	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			_CLASS_& _GETTER_()	\
			{							\
				return _NAME_;	\
			} 

//config file
#define CONFIG_FILE "config.json"

#endif //__GLOBALS_H__