#include "ASTrace.h"
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	as_trace_init(GetModuleHandle(0));

	while (1)
	{
		BYTE	theBuffer[121];

		as_trace_TL(0x0100, "This is a test.");
		as_trace_TL(0x0101, "%#2hx.", 4);
		as_trace_TL(0x0100, "[\\n\\r\\t] = [\n\r\t]");
		as_trace_TL(0x0100, "Specials: [ιθ]");
		theBuffer[0] = 0xE9;
		theBuffer[1] = 0xE8;
		as_trace_TL(0x0100, "Specials in a buffer:");
		as_trace_B(0x0200, theBuffer, 2);

		{
			int i;

			for (i = 0; i < sizeof(theBuffer) - 1; i++)
				theBuffer[i] = i;
			theBuffer[sizeof(theBuffer) - 1] = 0;
			as_trace_TL(0x0100, "Buffer in ASCII: %s", theBuffer + 1);
			as_trace_T(0x0100, "Buffer in HEXA");
			as_trace_B(0x0200, theBuffer, sizeof(theBuffer));
		}
		printf("Press any key to try again.\n");
		if (_getch() == 27)
			break;
	}

	as_trace_close();
	return 0;
}
