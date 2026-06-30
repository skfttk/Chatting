#include "Define.h"
#include "Server.h"

int main()
{
	Server m_Server;

	if (!m_Server.Start_Server())
		return -1;

	while (true)
	{
		if (!m_Server.Accept_ClntSock())
			continue;

		m_Server.ProcessClinet();
	}

	return 0;
}