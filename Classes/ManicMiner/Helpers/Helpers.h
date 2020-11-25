//////////////////
/// Bib
//////////////////
///
#ifndef HELPERS_H
#define HELPERS_H

// A template to allow for ease of deletion of pointers and array of pointers
template <typename T> void safeDelete( T*& a )
{
	delete a;
	a = nullptr;
}

template <typename T> void safeDeleteArray( T*& a )
{
	delete[] a;
	a = nullptr;
}


#ifdef _DEBUG
#define ASSERT_CHECK(param) assert(param)
#endif

#endif // !HELPERS_H
