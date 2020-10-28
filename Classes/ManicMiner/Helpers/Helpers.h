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
#endif // !HELPERS_H
