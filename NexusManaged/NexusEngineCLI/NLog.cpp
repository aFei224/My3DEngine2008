#include "StdAfx.h"
#include "NLog.h"

namespace NexusEngine
{
	NLogger::NLogger(void)
	{
	}

	void NLogger::WriteString( LogType t, System::String^ message )
	{
		pin_ptr<const wchar_t> szMessage = PtrToStringChars(message);		
		nlog::instance()->write(static_cast<nexus::ELogType>(t), szMessage);
	}

	void NLogOutput::WriteString( LogType t, System::String^ message )
	{
		if(m_NativeOutput != NULL)
		{
			pin_ptr<const wchar_t> szMessage = PtrToStringChars(message);		
			m_NativeOutput->write_string(static_cast<nexus::ELogType>(t), szMessage);
		}
	}

	NFileLogOutput::NFileLogOutput( System::String^ fileName ) 
		: NLogOutput( NULL )
	{
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(fileName);		
		nlog_output* new_output = new nfile_listener(szFileName);
		NativePtr = new_output;
	}

	NStderrLogOutput::NStderrLogOutput() : NLogOutput( new nstderr_listener())
	{

	}

	NConsoleLogOutput::NConsoleLogOutput(): NLogOutput( new nconsole_listener())
	{

	}
}//namespace NexusEngine