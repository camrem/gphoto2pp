#include "camera_file_wrapper.hpp"

#include "helper_gphoto2.hpp"
#include "camera_file_type_wrapper.hpp"

#ifdef GPHOTO_LESS_25
#include "exceptions.hpp"
#endif

#include "log.h"

namespace gphoto2
{
#include <gphoto2/gphoto2-file.h>
}

namespace gphoto2pp
{
	CameraFileWrapper::CameraFileWrapper()
		: m_cameraFile(nullptr)
	{
		FILE_LOG(logINFO) << "CameraFileWrapper Constructor";
		
		gphoto2pp::checkResponse(gphoto2::gp_file_new(&m_cameraFile),"gp_file_new");
	}

	CameraFileWrapper::~CameraFileWrapper()
	{
		FILE_LOG(logINFO) << "CameraFileWrapper Destructor";
		
		if(m_cameraFile != nullptr)
		{
			gphoto2pp::checkResponseSilent(gphoto2::gp_file_unref(m_cameraFile),"gp_file_unref");
			m_cameraFile = nullptr;
		}
	}
	
	CameraFileWrapper::CameraFileWrapper(CameraFileWrapper&& other)
		: m_cameraFile(other.m_cameraFile)
	{
		FILE_LOG(logINFO) << "CameraFileWrapper move Constructor";
		
		other.m_cameraFile = nullptr;
	}
	
	CameraFileWrapper& CameraFileWrapper::operator=(CameraFileWrapper&& other)
	{
		FILE_LOG(logINFO) << "CameraFileWrapper move assignment operator";
		
		if(this != &other)
		{
			// Release current objects resource
			if(m_cameraFile != nullptr)
			{
				gphoto2pp::checkResponse(gphoto2::gp_file_unref(m_cameraFile),"gp_file_unref");
			}
			
			// Steal or "move" the other objects resource
			m_cameraFile = other.m_cameraFile;
			
			// Unreference the other objects resource, so it's destructor doesn't unreference it
			other.m_cameraFile = nullptr;
		}
		return *this;
	}
	
	CameraFileWrapper::CameraFileWrapper(const CameraFileWrapper& other)
		: m_cameraFile(other.m_cameraFile)
	{
		FILE_LOG(logINFO) << "CameraFileWrapper copy Constructor";
		
		// Because we now refer to the same file as "other", we need to add to it's reference count
		if(m_cameraFile != nullptr)
		{
			gphoto2::gp_file_ref(m_cameraFile);
		}
	}

	CameraFileWrapper& CameraFileWrapper::operator=(const CameraFileWrapper& other)
	{
		FILE_LOG(logINFO) << "CameraFileWrapper copy assignment operator";
		
		// Check for self assignment
		if(this != &other)
		{
			// Release current objects resource
			if(m_cameraFile != nullptr)
			{
				gphoto2::gp_file_ref(m_cameraFile);
				m_cameraFile = nullptr;
			}
			
			// copy the other objects pointer
			m_cameraFile = other.m_cameraFile;
			
			if(m_cameraFile != nullptr)
			{
				gphoto2::gp_file_ref(m_cameraFile);
			}
		}
		return *this;
	}
	
	gphoto2::_CameraFile* CameraFileWrapper::getPtr() const
	{
		return m_cameraFile;
	}

	std::vector<char> CameraFileWrapper::getDataAndSize() const
	{
		FILE_LOG(logDEBUG) << "CameraFileWrapper getDataAndSize";
		
		const char* buffer = nullptr;
		unsigned long int size;
		
		gphoto2pp::checkResponse(gphoto2::gp_file_get_data_and_size(m_cameraFile,&buffer,&size),"gp_file_get_data_and_size");
		
		FILE_LOG(logDEBUG) << "bufferSize: '"<<size<<"'";
		
		return std::vector<char>(buffer, buffer+size);
	}
	
	void CameraFileWrapper::setDataAndSize(const std::vector<char>& file)
	{
		FILE_LOG(logDEBUG) << "CameraFileWrapper setDataAndSize copy";
		
		char* myCopy = new char[file.size()];
		
		try
		{
			std::copy(file.begin(), file.end(), myCopy);	
			gphoto2pp::checkResponse(gphoto2::gp_file_set_data_and_size(m_cameraFile,myCopy,file.size()),"gp_file_set_data_and_size");
			// We don't delete myCopy because ownership was transferred to the m_cameraFile struct along with the contents
		}
		catch (...)
		{
			// Just in case, we catch any exception and delete our myCopy, because we can assume it failed and we don't want that memory leaking
			delete[] myCopy;
			throw;
		}
	}

	std::string CameraFileWrapper::getMimeType() const
	{
		const char* temp = nullptr;
		
		gphoto2pp::checkResponse(gphoto2::gp_file_get_mime_type(m_cameraFile, &temp),"gp_file_get_mime_type");
		
		return std::string(temp);
	}

	void CameraFileWrapper::setMimeType(const std::string& mimeType)
	{
		gphoto2pp::checkResponse(gphoto2::gp_file_set_mime_type(m_cameraFile, mimeType.c_str()),"gp_file_set_mime_type");
	}

	std::string CameraFileWrapper::getFileName() const
	{
		const char* temp = nullptr;
		
		gphoto2pp::checkResponse(gphoto2::gp_file_get_name(m_cameraFile, &temp),"gp_file_get_name");
		
		return std::string(temp);
	}

	void CameraFileWrapper::setFileName(const std::string& fileName)
	{
		gphoto2pp::checkResponse(gphoto2::gp_file_set_name(m_cameraFile, fileName.c_str()),"gp_file_set_name");
	}

	void CameraFileWrapper::detectMimeType() const
	{
		gphoto2pp::checkResponse(gphoto2::gp_file_detect_mime_type(m_cameraFile),"gp_file_detect_mime_type");
	}

	void CameraFileWrapper::adjustNameForMimeType()
	{
		gphoto2pp::checkResponse(gphoto2::gp_file_adjust_name_for_mime_type(m_cameraFile),"gp_file_adjust_name_for_mime_type");
	}


	std::string CameraFileWrapper::getNameByType(const std::string& baseName, const CameraFileTypeWrapper& type) const
	{
#ifdef GPHOTO_LESS_25
		throw exceptions::InvalidLinkedVersionException("You are using a version of gphoto2 that doesn't support this command. Please link to gphoto 2.5 or greater");
#else
		char* temp = nullptr;
		
		gphoto2pp::checkResponse(gphoto2::gp_file_get_name_by_type(m_cameraFile, baseName.c_str(), static_cast<gphoto2::CameraFileType>(type), &temp),"gp_file_get_name_by_type");
		
		return std::string(temp);
#endif
	}


	void CameraFileWrapper::save(const std::string& filename) const
	{
		// Not sure if we should expose this. in gphoto2-file.h it mentions this method was never meant for public api usage, but it is handy and lots of people use it... I will add it now for completness
		gphoto2pp::checkResponse(gphoto2::gp_file_save(m_cameraFile, filename.c_str()),"gp_file_save");
		
	}
	
	std::time_t CameraFileWrapper::getMtime() const
	{
		time_t time;
		
		gphoto2pp::checkResponse(gphoto2::gp_file_get_mtime(m_cameraFile, &time),"gp_file_get_mtime");
		
		return time;
	}
	
	void CameraFileWrapper::setMtime(std::time_t mtime)
	{
		gphoto2pp::checkResponse(gphoto2::gp_file_set_mtime(m_cameraFile, mtime),"gp_file_set_mtime");
	}
}

