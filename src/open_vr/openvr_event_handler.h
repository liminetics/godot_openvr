#ifndef OPENVR_EVENT_HANDLER_H
#define OPENVR_EVENT_HANDLER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vmap.hpp>

#include "openvr_data.h"

namespace godot {
class OpenVREventHandler : public Node {
	GDCLASS(OpenVREventHandler, Node)

	friend class openvr_data;

public:
	enum OpenVREventDataType { // Derived from the VREvent_Data_t union.
		None, // Documented to contain no data.
		Unknown = None, // Undocumented and not yet determined. Please fix these entries if you can!

		// Reserved, // Unused, here for completeness.
		Controller,
		Mouse,
		Scroll,
		Process,
		Notification,
		Overlay,
		Status,
		Keyboard,
		Ipd,
		Chaperone,
		PerformanceTest,
		TouchPadMove,
		SeatedZeroPoseReset,
		Screenshot,
		ScreenshotProgress,
		ApplicationLaunch,
		EditingCameraSurface,
		MessageOverlay,
		Property,
		HapticVibration,
		WebConsole,
		InputBindingLoad,
		InputActionManifestLoad,
		SpatialAnchor,
		ProgressUpdate,
		ShowUI,
		ShowDevTools,
		HDCPError,
		AudioVolumeControl,
		AudioMuteControl,
	};

	// Register an event type which exists in a newer version of OpenVR.
	void register_event_signal(uint32_t p_event_id, OpenVREventDataType p_type, String p_signal_name);

private:
	openvr_data *ovr;

	struct vr_event {
		OpenVREventDataType data_type;
		StringName signal_name;
	};

protected:
	static VMap<uint32_t, vr_event> event_signals;

	static void _bind_methods();
	void handle_event(vr::VREvent_t event);

	OpenVREventHandler();
	~OpenVREventHandler();
};

} // namespace godot

VARIANT_ENUM_CAST(OpenVREventHandler::OpenVREventDataType);

#endif /* OPENVR_EVENT_HANDLER_H */
