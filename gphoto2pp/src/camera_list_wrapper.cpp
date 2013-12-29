#include "camera_list_wrapper.hpp"

#include "helper_gphoto2.hpp"

#include "log.h"

namespace gphoto2
{
#include <gphoto2/gphoto2-list.h>
}

namespace gphoto2pp
{

	CameraListWrapper::CameraListWrapper()
		: m_cameraList(nullptr)
	{
		FILE_LOG(logINFO) << "CameraListWrapper Constructor";
		
		gphoto2pp::checkResponse(gphoto2::gp_list_new(&m_cameraList),"gp_list_new");
	}

	CameraListWrapper::~CameraListWrapper()
	{
		FILE_LOG(logINFO) << "~CameraListWrapper Destructor";
		
		if(m_cameraList != nullptr) // Added this line so the move constructor wouldn't dispose of the object if it was reassigned
		{
			gphoto2pp::checkResponseSilent(gphoto2::gp_list_unref(m_cameraList),"gp_list_unref");
			m_cameraList = nullptr;
		}
	}
	
	CameraListWrapper::CameraListWrapper(CameraListWrapper&& other)
		: m_cameraList(other.m_cameraList)
	{
		FILE_LOG(logINFO) << "CameraListWrapper move Constructor";
		
		other.m_cameraList = nullptr;
	}
	
	CameraListWrapper& CameraListWrapper::operator=(CameraListWrapper&& other)
	{
		FILE_LOG(logINFO) << "CameraListWrapper move assignment operator";
		
		if(this != &other)
		{
			// Release current objects resource
			if(m_cameraList != nullptr)
			{
				FILE_LOG(logDEBUG) << "CameraListWrapper move assignment - existing cameraList is not null";
				gphoto2pp::checkResponse(gphoto2::gp_list_unref(m_cameraList),"gp_list_unref");
			}
			
			// Steal or "move" the other objects resource
			m_cameraList = other.m_cameraList;
			
			// Unreference the other objects resource, so it's destructor doesn't unreference it
			other.m_cameraList = nullptr;
		}
		return *this;
	}
	
	CameraListWrapper::CameraListWrapper(const CameraListWrapper& other)
		: m_cameraList(other.m_cameraList)
	{
		FILE_LOG(logINFO) << "CameraListWrapper copy Constructor";
		
		// Because we now refer to the same cameralist as "other", we need to add to it's reference count
		if(m_cameraList != nullptr)
		{
			gphoto2::gp_list_ref(m_cameraList);
		}
	}
	
	CameraListWrapper& CameraListWrapper::operator=(const CameraListWrapper& other)
	{
		FILE_LOG(logINFO) << "CameraListWrapper copy assignment operator";
		
		if(this != &other)
		{
			// Release current objects resource
			if(m_cameraList != nullptr)
			{
				gphoto2pp::checkResponse(gphoto2::gp_list_unref(m_cameraList),"gp_list_unref");
			}
			
			// copy the other objects pointer
			m_cameraList = other.m_cameraList;
			
			// Add reference count to the copied cameraList
			if(m_cameraList != nullptr)
			{
				gphoto2pp::checkResponse(gphoto2::gp_list_ref(m_cameraList),"gp_list_ref");
			}
		}
		return *this;
	}
	
	gphoto2::_CameraList* CameraListWrapper::getPtr() const
	{
		return m_cameraList;
	}

	int CameraListWrapper::count() const
	{
		return gphoto2pp::checkResponse(gphoto2::gp_list_count(m_cameraList),"gp_list_count");
	}

	std::string CameraListWrapper::getName(int index) const
	{
		const char* temp = nullptr;
		
		gphoto2pp::checkResponse(gphoto2::gp_list_get_name(m_cameraList, index, &temp),"gp_list_get_name");
		
		if(temp == nullptr)
		{
			return std::string();
		}
		else
		{
			return std::string(temp);
		}
	}

	std::string CameraListWrapper::getValue(int index) const
	{
		const char* temp = nullptr;
		
		gphoto2pp::checkResponse(gphoto2::gp_list_get_value(m_cameraList, index, &temp),"gp_list_get_value");
		
		if(temp == nullptr) // sometimes it returns null, so we will just consider that an empty string
		{
			return std::string();
		}
		else
		{
			return std::string(temp);
		}
	}
	
	void CameraListWrapper::append(const std::string& name, const std::string& value)
	{
		gphoto2pp::checkResponse(gphoto2::gp_list_append(m_cameraList, name.c_str(), value.c_str()),"gp_list_append");
	}
	
	void CameraListWrapper::reset()
	{
		gphoto2pp::checkResponse(gphoto2::gp_list_reset(m_cameraList),"gp_list_reset");
	}
	
	void CameraListWrapper::sort()
	{
		gphoto2pp::checkResponse(gphoto2::gp_list_sort(m_cameraList),"gp_list_sort");
	}
	
	int CameraListWrapper::findByName(const std::string& name) const
	{
		int index;
		
		gphoto2pp::checkResponse(gphoto2::gp_list_find_by_name(m_cameraList, &index, name.c_str()),"gp_list_find_by_name");
		
		return index;
	}
	
	void CameraListWrapper::setName(int index, const std::string& name)
	{
		gphoto2pp::checkResponse(gphoto2::gp_list_set_name(m_cameraList, index, name.c_str()),"gp_list_set_name");
	}

	void CameraListWrapper::setValue(int index, const std::string& value)
	{
		gphoto2pp::checkResponse(gphoto2::gp_list_set_value(m_cameraList, index, value.c_str()),"gp_list_set_value");
	}
	
	std::pair<std::string, std::string> CameraListWrapper::getPair(int index) const
	{
		return std::make_pair(getName(index), getValue(index));
	}
		
	std::pair<std::string, std::string> CameraListWrapper::getPairByName(const std::string& name) const
	{
		auto index = findByName(name);
		
		return std::make_pair(getName(index), getValue(index));
	}
}

