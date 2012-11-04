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
#include <boost/thread/thread.hpp>


boost::asio::io_service ioService_;


class SimpleCommunicatorTest : public ::testing::Test
{

public:
	SimpleCommunicatorTest()
	{
		boost::thread t(&SimpleCommunicatorTest::startServer);
	}

	virtual ~SimpleCommunicatorTest()
	{}

	virtual void SetUp()
	{}

	virtual void TearDown()
	{}

private:

	static string handler(const string& s)
	{
		string x = s;
		x += "x";
		return x;
	}


	static void startServer()
	{
		AsioServer as(ioService_, SimpleCommunicatorTest::handler);
	}

};


string genRandomString(size_t max_len)
{
	size_t strLen = (rand() % max_len) + 1;
	string s;
	s.resize(strLen);

	for (string::iterator i = s.begin(); i != s.end(); i++)
		*i = 'a' + (rand() % 26);

	return s;

}


TEST_F(SimpleCommunicatorTest, simpleTestCommunication)
{
	AsioClient ac(ioService_);
	string res = ac.remoteExecute("1234");
	ASSERT_EQ(res, "1234x");

}


TEST_F(SimpleCommunicatorTest, smokeTest)
{

	AsioClient ac(ioService_);

	for (size_t i = 0; i < 0x100; i++)
	{
		string s = genRandomString(i + 1);
		string sAndX = ac.remoteExecute(s);
		ASSERT_EQ(s + "x", sAndX);
	}

}
