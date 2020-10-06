#include <windows.h>

#include "Helpers.h"

namespace Helpers
{
	namespace Keyboard
	{
		bool isKeyPressed(int virtualKey)
		{
			virtualKey = toupper(virtualKey);

			// Mask out all bits apart from BIT 15 because thats the way GetAsyncKeyState works
			// From https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
			return (bool)(GetAsyncKeyState(virtualKey) & 0x8000);
		}
	}

}
