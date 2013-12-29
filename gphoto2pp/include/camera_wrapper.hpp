#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "observer.hpp"

#include <string>
#include <iosfwd>
#include <future>
#include <mutex>
#include <memory>

namespace gphoto2
{
	struct _Camera;
	struct _GPContext;
}

namespace gphoto2pp
{
	//Forward Declarations
	enum class CameraEventTypeWrapper : int;
	enum class CameraFileTypeWrapper : int;
	enum class CameraCaptureTypeWrapper : int;
	
	struct CameraFilePathWrapper;
	
	class CameraFileWrapper;
	class CameraWidgetWrapper;
	class WindowWidget;
	class CameraListWrapper;
	
	class CameraWrapper
	{
	public:
		/**
		 * @brief Create a Camera without prior using autoDetect static method. This method will automatically choose the first camera it finds
		 */
		CameraWrapper();
		
		/**
		 * @brief Connects and initializes using the provided model and port.
		 */
		CameraWrapper(const std::string& model, const std::string& port);

		~CameraWrapper();
		
		// Move constructor and move assignment are allowed.
		CameraWrapper(CameraWrapper&& other);
		CameraWrapper& operator=(CameraWrapper&& other);
		
		// This is not copyable. the t_Camera object is sort of a singleton. We don't want multiple commands being issued to one t_Camera resource.
		CameraWrapper(const CameraWrapper& other) = delete;
		CameraWrapper& operator=( const CameraWrapper& ) = delete;
		
		/**
		 * \brief Gets the complete summary of the currently connected camera.
		 * This summary indicates numerous details about make, model, and configurable abilities.
		 * \return the summary
		 * \note Direct wrapper for <tt>gp_camera_get_summary(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		std::string getSummary() const;
		
		/**
		 * \brief Captures a preview image from the camera.
		 * This capture type might not be supported by all cameras (requires a live view/mirror lockup mode for continuous captures). The image does not persist on the camera.
		 * \return the image captured
		 * \note Direct wrapper for <tt>gp_camera_capture_preview(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		CameraFileWrapper capturePreview();
		
		/**
		 * \brief Captures a file from the camera.
		 * \param[in]	captureType	of file to retrieve from the camera
		 * \return the file path
		 * \note Direct wrapper for <tt>gp_camera_capture(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		CameraFilePathWrapper capture(const CameraCaptureTypeWrapper& captureType);
		
		/**
		 * \brief Triggers the camera to take a picture (similar to a remote shutter release).
		 * The image is saved to the camera's portable storage for retrieval later
		 * \note Direct wrapper for <tt>gp_camera_trigger_capture(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void triggerCapture();
		
		/**
		 * \brief Queries all the properties/abilities on the camera and compiles them in a Widget N-way tree hierarchy.
		 * \return the Root widget (which will always be of type Window Widget)
		 * \note Direct wrapper for <tt>gp_camera_get_config(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		WindowWidget getConfig() const;
		
		/**
		 * \brief Sets the widgets to the provided settings.
		 * It's important to note, that if camera settings change (manually by physical interaction), and then you call this method with the old settings, it will change the camera back to the old settings. It's best practice to query the camera, change the settings, and then immediately set the config again.
		 * \param[in]	cameraWidget	to traverse and write all settings to the camera
		 * \return the Root widget (which will always be of type Window Widget)
		 * \note Direct wrapper for <tt>gp_camera_set_config(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void setConfig(const CameraWidgetWrapper& cameraWidget);
		
		//Filesystem Operations
		/**
		 * \brief Lists all files in the provided folder
		 * \param[in]	folder	to list all files in
		 * \return the list of files
		 * \note Direct wrapper for <tt>gp_camera_folder_list_files(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		CameraListWrapper folderListFiles(const std::string& folder) const;
		
		/**
		 * \brief Lists all folders in the provided folder
		 * \param[in]	folder	to list all folders in
		 * \return the list of folders
		 * \note Direct wrapper for <tt>gp_camera_folder_list_folders(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		CameraListWrapper folderListFolders(const std::string& folder) const;
		
		/**
		 * \brief Delete all files in the provided folder
		 * \param[in]	folder	to delete all files
		 * \note Direct wrapper for <tt>gp_camera_folder_delete_all(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void folderDeleteAll(const std::string& folder);
		
		/**
		 * \brief Write a file in the provided folder
		 * \param[in]	folder	to write the new file in
		 * \param[in]	fileName	for the new file to be written
		 * \param[in]	fileType	for the new file to be written
		 * \param[in]	cameraFile	contains the new file to be written to the folder
		 * \note Direct wrapper for <tt>gp_camera_folder_put_file(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void folderPutFile(const std::string& folder, const std::string& fileName, const CameraFileTypeWrapper& fileType, CameraFileWrapper cameraFile);
		
		/**
		 * \brief Make a new folder in the provided directory
		 * \param[in]	folder	to make the new folder in
		 * \param[in]	name	for the new folder
		 * \note Direct wrapper for <tt>gp_camera_folder_make_dir(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void folderMakeDir(const std::string& folder, const std::string& name);
		
		/**
		 * \brief Remove a directory from the provided folder
		 * \param[in]	folder	containing the folder to remove
		 * \param[in]	name	of the folder to remove
		 * \note Direct wrapper for <tt>gp_camera_folder_remove_dir(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void folderRemoveDir(const std::string& folder, const std::string& name);
		
		/**
		 * \brief Retrieve a file from the camera
		 * \param[in]	folder	containing the file to get
		 * \param[in]	fileName	of the file to retrieve
		 * \param[in]	fileType	of the file to retrieve
		 * \return the file
		 * \note Direct wrapper for <tt>gp_camera_file_get(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		CameraFileWrapper fileGet(const std::string& folder, const std::string& fileName, const CameraFileTypeWrapper& fileType) const;
		
		/**
		 * \brief Delete a file from the camera
		 * \param[in]	folder	containing the file to delete
		 * \param[in]	fileName	of the file to delete
		 * \note Direct wrapper for <tt>gp_camera_file_get(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		void fileDelete(const std::string& folder, const std::string& fileName) const;
		
		/**
		 * \brief Helper method used to subscribe to Camera Wait For events.
		 * This method should be used to setup all the callbacks necessary before calling startListeningForEvents.
		 * Provide your callback method with lambdas or static/instance methods (use std::bind).
		 * \par Event Types:
		 * - FileAdded
		 * - FolderAdded
		 * - CaptureComplete
		 * - Timeout
		 * - Unknown
		 * \param[in]	event	type to subscribe to
		 * \param[in]	func	callback which will be called each time the event type is triggered
		 * \note Helper structure that wraps <tt>gp_camera_wait_for_event(...)</tt>
		 * \throw GPhoto2pp::exceptions::gphoto2_exception
		 */
		observer::Registration subscribeToCameraEvent(const CameraEventTypeWrapper& event, std::function<void(const CameraFilePathWrapper&, const std::string&)> func);
		
		/**
		 * \brief Starts monitoring the camera events
		 * You must subscribe to at least one event type and then perform some action on the camera to see this in action.
		 * \return true if started listening for events, false if already listening to events
		 */
		bool startListeningForEvents();
		
		/**
		 * \brief Signals the thread to stop listening.
		 * If there is no thread, it leaves the method immediately, if there is a thread running, it is a blocking call which will not return until the thread exits.
		 */
		void stopListeningForEvents();
		
	private:
		/**
		 * \brief Initializes the camera by connecting to the first camera found.
		 */
		void initialize();
		
		/**
		 * \brief Initializes a specific camera. You can obtain the model and port using the gphoto2pp::helper::autoDetectAll
		 * \param[in]	model	of the camera to connect to
		 * \param[in]	port	to find the camera and connect to it through
		 */
		void initialize(const std::string& model, const std::string& port);
		
		gphoto2::_Camera* m_camera = nullptr;
		
		std::shared_ptr<gphoto2::_GPContext> m_context;

		std::string m_model;
		std::string m_port;
		
		observer::SubjectEvent<CameraEventTypeWrapper, void(const CameraFilePathWrapper&, const std::string&)> m_cameraEvents;
		
		std::atomic<bool> m_listenForEvents;
		std::future<bool> m_listenForEventSignalFuture;
		
		mutable std::mutex m_cameraIOMutex;
	};

}

#endif // CAMERA_HPP
