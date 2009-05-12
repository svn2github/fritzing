/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2009 Fachhochschule Potsdam - http://fh-potsdam.de

Fritzing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fritzing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fritzing.  If not, see <http://www.gnu.org/licenses/>.

********************************************************************

$Revision$:
$Author$:
$Date$

********************************************************************/



#include "fapplication.h"
#include "debugdialog.h"

#ifdef Q_WS_WIN
#ifndef QT_NO_DEBUG
#include "windows.h"
#endif
#endif

int main(int argc, char *argv[])
{

#ifdef Q_WS_WIN
#ifndef QT_NO_DEBUG
	HANDLE hLogFile;
	hLogFile = CreateFile(L"fritzing_leak_log.txt", GENERIC_WRITE,
		  FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		  FILE_ATTRIBUTE_NORMAL, NULL);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, hLogFile);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, hLogFile);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, hLogFile);
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(24378);
#endif
#endif

	FApplication * app = new FApplication(argc, argv);
	//DebugDialog::setDebugLevel(DebugDialog::Error);
	int result = app->startup(argc, argv);
	if (result == 0) {
		result = app->exec();
	}
	app->finish();

	delete app;
	return result;
}
