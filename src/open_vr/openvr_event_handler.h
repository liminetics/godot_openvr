#ifndef OPENVR_EVENT_HANDLER_H
#define OPENVR_EVENT_HANDLER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vmap.hpp>

#include "openvr_data.h"

namespace godot {
class OpenVREventHandler : public Node {
	GDCLASS(OpenVREventHandler, Node)

	friend class openvr_data;

private:
	openvr_data *ovr;

	enum OpenVREventDataType { // Derived from the VREvent_Data_t union.
		Reserved, // VREvent_Reserved_t should never arrive, used here to indicate the data should not be decoded.
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

#endif /* OPENVR_EVENT_HANDLER_H */
