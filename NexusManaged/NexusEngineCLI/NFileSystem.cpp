#include "StdAfx.h"
#include "NFileSystem.h"

namespace NexusEngine
{
	//---------------------------------------------------------------------------------------
	NFile::NFile(nexus::nfile::ptr nativeObj):NativeObjectOwnedBySharedPtr(nativeObj)
	{}

	nexus::nfile* NFile::NativePtr::get()
	{	
		return dynamic_cast<nexus::nfile*>(this->RawPtr);		 
	}

	//---------------------------------------------------------------------------------------
	NFileSystem::NFileSystem(System::String^ nativeClassName)
	{
		pin_ptr<const wchar_t> szClassName = PtrToStringChars(nativeClassName);		
		NativeObjectOwnedRaw^ nativeObj = gcnew NativeObjectOwnedRaw(
			nconstruct<nfile_system>(szClassName));
		m_nativeObj = nativeObj;
	}

	NFileSystem::NFileSystem(INativeObj^ nativeObj):m_nativeObj(nativeObj)
	{}

	nfile_system* NFileSystem::NativePtr::get()
	{
		return dynamic_cast<nfile_system*>(m_nativeObj->GetRawPtr());
	}

	int NFileSystem::GetNumPackage()
	{
		return NativePtr->get_num_package();
	}

	System::String^ NFileSystem::GetPackageName(int i)
	{
		return gcnew System::String(NativePtr->get_package_name(i).c_str());
	}

	void NFileSystem::QueryPackage(System::String^ pkgName, System::String^ path, NFileQuery^ fq)
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szPath = PtrToStringChars(path);		

		NativeFileQueryWrapper wrapper(fq);
		NativePtr->query_package(szPkgName, szPath, wrapper);
	}

	System::UInt32 NFileSystem::GetFileSize(System::String^ pkgName, System::String^ path)
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szPath = PtrToStringChars(path);		

		return NativePtr->get_file_size(szPkgName, szPath);
	}

	System::DateTime NFileSystem::GetFileTime(System::String^ pkgName, System::String^ path)
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szPath = PtrToStringChars(path);		

		time_t binTime = NativePtr->get_file_time(szPkgName, szPath);

		long long filetime = binTime * 10000000LL + 116444736000000000LL;
		return DateTime::FromFileTime(filetime);
	}

	System::String^ NFileSystem::GetFullPath(System::String^ pkgName, System::String^ fileName)
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(fileName);		

		nstring ret = NativePtr->get_full_path(szPkgName, szFileName);
		return gcnew System::String(ret.c_str());
	}

	bool NFileSystem::IsFileExist( System::String^ pkgName, System::String^ path )
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(path);		

		return NativePtr->file_exists(szPkgName,szFileName);
	}

	void NFileSystem::CreateDirectory( System::String^ pkgName, System::String^ path )
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(path);		
		NativePtr->create_directory(szPkgName,szFileName);
	}

	void NFileSystem::DeleteDirectory( System::String^ pkgName, System::String^ path )
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> szFileName = PtrToStringChars(path);		
		NativePtr->remove_directory(szPkgName,szFileName);
	}

	bool NFileSystem::RenameDirectory( System::String^ pkgName, System::String^ oldName,System::String^ newName )
	{
		pin_ptr<const wchar_t> szPkgName = PtrToStringChars(pkgName);		
		pin_ptr<const wchar_t> oldFileName = PtrToStringChars(oldName);		
		pin_ptr<const wchar_t> newFileName = PtrToStringChars(newName);		
		return NativePtr->rename_directory(szPkgName,oldFileName,newFileName);
	}

}//namespace NexusCore