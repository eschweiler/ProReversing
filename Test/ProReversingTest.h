/* ============================================================================
* Copyright (c) 2012, Sebastian Eschweiler <advanced(dot)malware<dot>analyst[at]gmail.com>
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the <organization> nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* =============================================================================
*/

#include <gtest/gtest.h>
#include <SimpleCommunicator.h>
#include <Debugger.h>
#include <boost/thread/thread.hpp>
#ifndef _WIN64
#include <Injector.h>
#endif

#include <PebBeingDebugged.h>
#include <PebHeapFlags.h>
#include <NtGlobalFlag.h>

#include <Windows.h>


class ProReversingTest : public ::testing::Test
{

public:
	ProReversingTest()
	{
		techniques_.push_back(auto_ptr<ProTechnique>(new PebBeingDebugged));
		techniques_.push_back(auto_ptr<ProTechnique>(new PebHeapFlags));
		techniques_.push_back(auto_ptr<ProTechnique>(new NtGlobalFlag));
	}

	virtual ~ProReversingTest()
	{}

	virtual void SetUp()
	{}

	virtual void TearDown()
	{}

	static vector<auto_ptr<ProTechnique>> techniques_;
	static boost::asio::io_service ioService_;

};

vector<auto_ptr<ProTechnique>> ProReversingTest::techniques_;
boost::asio::io_service ProReversingTest::ioService_;


DWORD startServer(Debugger* dbg = NULL)
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&startupInfo, sizeof startupInfo);
	startupInfo.cb = sizeof(STARTUPINFO);

	if (!CreateProcess("AntiReversing.exe", NULL, NULL, NULL, FALSE, dbg ? DEBUG_ONLY_THIS_PROCESS : 0, NULL, NULL, &startupInfo, &processInfo))
	{
		throw exception("Error: could not create TestApp\n");
	}

	if (dbg)
	{
		dbg->setProcessInfo(processInfo);
	}

	return processInfo.dwProcessId;
}

DWORD pid;


void executeTests(const string& expectedResult, const vector<string>& prepend_commands = vector<string>())
{
	AsioClient ac(ProReversingTest::ioService_);

	if (!prepend_commands.empty())
	{
		for(vector<string>::const_iterator i = prepend_commands.begin(); i != prepend_commands.end(); i++)
			ac.remoteExecute(*i);
	}

	for (vector<auto_ptr<ProTechnique>>::iterator i = ProReversingTest::techniques_.begin(); i != ProReversingTest::techniques_.end(); i++)
	{
		string result = ac.remoteExecute((*i)->name());

		cout << "checking " << (*i)->name() << ", asserting " << expectedResult << endl;
		ASSERT_EQ(result, expectedResult);
	}

	ac.remoteExecute("exit");

	TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid), 0);

}


TEST_F(ProReversingTest, antiReversingCheckWithoutDebugger)
{
	pid = startServer();

	executeTests("false");
	
 }



void startDebugging()
{
	Debugger dbg;
	pid = startServer(&dbg);

	dbg.debugLoop();

}


TEST_F(ProReversingTest, antiReversingCheckWithDebugger)
{
	boost::thread t(startDebugging);

	executeTests("true");

}


TEST_F(ProReversingTest, antiReversingCheckWithProReversingEnabled)
{
	boost::thread t(startDebugging);

	vector<string> additionalCommands;
	additionalCommands.push_back("enable");

	executeTests("false", additionalCommands);

}


TEST_F(ProReversingTest, antiReversingCheckWithProReversingEnabledThenDisabled)
{
	boost::thread t(startDebugging);

	vector<string> additionalCommands;
	additionalCommands.push_back("enable");
	additionalCommands.push_back("disable");

	executeTests("true", additionalCommands);

}


#ifndef _WIN64 // injection test currently works only with 32 bit code
TEST_F(ProReversingTest, antiReversingCheckWithDllInjected)
{
	boost::thread t(startDebugging);

	pid = startServer(false);

	Injector(pid, "ProReversing.dll");

	executeTests("false");
}

#endif


TEST_F(ProReversingTest, checkPebHeapFlags)
{
	PebHeapFlags p;
	p.enable();
	
	p.isAntiTechniqueStillWorking();
}
