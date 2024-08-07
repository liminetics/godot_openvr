#include "openvr_event_handler.h"
#include <godot_cpp/variant/utility_functions.hpp>

#define VREVENT_SIGNAL(vrevent_name, vrevent_type, source)                                                                    \
	{                                                                                                                         \
		String name = String(#vrevent_name).trim_prefix("vr::EVREventType::VREvent_");                                        \
		ADD_SIGNAL(MethodInfo(name,                                                                                           \
				PropertyInfo(Variant::INT, "eventAgeSeconds"),                                                                \
				PropertyInfo(Variant::INT, "trackedDeviceIndex"),                                                             \
				PropertyInfo(Variant::DICTIONARY, "data")));                                                                  \
		OpenVREventHandler::event_signals.insert(vrevent_name, (vr_event){ .data_type = vrevent_type, .signal_name = name }); \
	}

using namespace godot;

VMap<uint32_t, OpenVREventHandler::vr_event> OpenVREventHandler::event_signals;

void OpenVREventHandler::_bind_methods() {
	// The EVREvent "documentation" is incomplete: only a handful of the events have a comment describing which VREvent_Data_t
	// is attached to them. Here, we use `Unknown` for the ones that haven't yet been reverse engineered. `None` means it is
	// known to actually not include any data.
	//
	// Any entries which are determined via reverse engineering (or guessing) should have a comment indicating as much, just
	// to set the appropriate level of skepticism if bugs are traced back here.
	//
	// The last argument of the macro is provided to track the confidence in an entry being correct:
	// - "none" means it's a mystery, should appear with Unknown.
	// - "guess" means makes sense but hasn't been tested
	// - "tested" means it was a guess but has been tested and seems to return the correct info
	// - "header" means openvr.h says so
	//
	// These values are only for human reference. Feel free to add another if it's more nuanced.
	//
	// The whitespace below matches Valve's so this is a little easier to skim alongside openvr.h.

	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceActivated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceDeactivated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUpdated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUserInteractionStarted, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUserInteractionEnded, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_IpdChanged, OpenVREventDataType::Ipd, guess);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnterStandbyMode, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LeaveStandbyMode, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceRoleChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WatchdogWakeUpRequested, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LensDistortionChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PropertyChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WirelessDisconnect, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WirelessReconnect, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonPress, OpenVREventDataType::Controller, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonUnpress, OpenVREventDataType::Controller, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonTouch, OpenVREventDataType::Controller, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonUntouch, OpenVREventDataType::Controller, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Modal_Cancel, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseMove, OpenVREventDataType::Mouse, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseButtonDown, OpenVREventDataType::Mouse, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseButtonUp, OpenVREventDataType::Mouse, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FocusEnter, OpenVREventDataType::Overlay, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FocusLeave, OpenVREventDataType::Overlay, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScrollDiscrete, OpenVREventDataType::Scroll, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TouchPadMove, OpenVREventDataType::Mouse, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayFocusChanged, OpenVREventDataType::Overlay, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ReloadOverlays, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScrollSmooth, OpenVREventDataType::Scroll, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LockMousePosition, OpenVREventDataType::Mouse, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_UnlockMousePosition, OpenVREventDataType::Mouse, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusCaptured, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusReleased, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationChanged, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusChanged, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationUsingWrongGraphicsAdapter, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ActionBindingReloaded, OpenVREventDataType::Process, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_HideRenderModels, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowRenderModels, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationStateChanged, OpenVREventDataType::None, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneAppPipeDisconnected, OpenVREventDataType::Process, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_ConsoleOpened, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ConsoleClosed, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayShown, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayHidden, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardActivated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardDeactivated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardRequested, OpenVREventDataType::Overlay, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ResetDashboard, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ImageLoaded, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowKeyboard, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_HideKeyboard, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayGamepadFocusGained, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayGamepadFocusLost, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlaySharedTextureChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotTriggered, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ImageFailed, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardOverlayCreated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SwitchGamepadFocus, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_RequestScreenshot, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotTaken, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotFailed, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SubmitScreenshotToDashboard, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotProgressToDashboard, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_PrimaryDashboardDeviceChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RoomViewShown, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RoomViewHidden, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowUI, OpenVREventDataType::ShowUI, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowDevTools, OpenVREventDataType::ShowDevTools, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopViewUpdating, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopViewReady, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_StartDashboard, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ElevatePrism, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayClosed, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardThumbChanged, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopMightBeVisible, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopMightBeHidden, OpenVREventDataType::Unknown, none);

	// TODO: haven't updated openvr yet, these don't exist. Use them to test dynamic adding.
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_MutualSteamCapabilitiesChanged, OpenVREventDataType::Unknown, none);
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayCreated, OpenVREventDataType::Overlay, header);
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayDestroyed, OpenVREventDataType::Overlay, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Shown, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Hidden, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_BeginInteraction, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Destroyed, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Quit, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessQuit, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_QuitAcknowledged, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DriverRequestedQuit, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RestartRequested, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InvalidateSwapTextureSets, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneDataHasChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneUniverseHasChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneTempDataHasChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SeatedZeroPoseReset, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneFlushCache, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneRoomSetupStarting, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneRoomSetupFinished, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_StandingZeroPoseReset, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_AudioSettingsHaveChanged, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_BackgroundSettingHasChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_CameraSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ReprojectionSettingHasChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ModelSkinSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnvironmentSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PowerSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnableHomeAppSettingsHaveChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SteamVRSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LighthouseSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_NullSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_UserInterfaceSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_NotificationsSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerfSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WebInterfaceSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackersSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LastKnownSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DismissedWarningsSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_GpuSpeedSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WindowsMRSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OtherSectionSettingChanged, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_AnyDriverSettingsChanged, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_StatusUpdate, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_WebInterface_InstallDriverCompleted, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_MCImageUpdated, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_FirmwareUpdateStarted, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FirmwareUpdateFinished, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardClosed, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardCharInput, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardDone, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardOpened_Global, OpenVREventDataType::Keyboard, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardClosed_Global, OpenVREventDataType::Keyboard, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_ApplicationListUpdated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ApplicationMimeTypeLoad, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessConnected, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessDisconnected, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ChaperoneBoundsShown, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ChaperoneBoundsHidden, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayDisconnected, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayReconnected, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_HDCPError, OpenVREventDataType::HDCPError, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ApplicationNotResponding, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ApplicationResumed, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_OutOfVideoMemory, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayModeNotSupported, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_StageOverrideReady, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_RequestDisconnectReconnect, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_StartVideoStream, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_StopVideoStream, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_PauseVideoStream, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_ResumeVideoStream, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_EditingSurface, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_EnableCapture, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_DisableCapture, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_FidelityLevel, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_MessageOverlay_Closed, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MessageOverlayCloseRequested, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_HapticVibration, OpenVREventDataType::HapticVibration, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingLoadFailed, OpenVREventDataType::InputBindingLoad, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingLoadSuccessful, OpenVREventDataType::InputBindingLoad, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ActionManifestReloaded, OpenVREventDataType::None, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ActionManifestLoadFailed, OpenVREventDataType::InputActionManifestLoad, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ProgressUpdate, OpenVREventDataType::ProgressUpdate, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_TrackerActivated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingsUpdated, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingSubscriptionChanged, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_PoseUpdated, OpenVREventDataType::SpatialAnchor, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_DescriptorUpdated, OpenVREventDataType::SpatialAnchor, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_RequestPoseUpdate, OpenVREventDataType::SpatialAnchor, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_RequestDescriptorUpdate, OpenVREventDataType::SpatialAnchor, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_SystemReport_Started, OpenVREventDataType::Unknown, none);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Monitor_ShowHeadsetView, OpenVREventDataType::Process, header);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Monitor_HideHeadsetView, OpenVREventDataType::Process, header);

	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetSpeakersVolume, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetSpeakersMute, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetMicrophoneVolume, OpenVREventDataType::Unknown, none);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetMicrophoneMute, OpenVREventDataType::Unknown, none);
}

void OpenVREventHandler::handle_event(vr::VREvent_t event) {
	if (!OpenVREventHandler::event_signals.has(event.eventType)) {
		return;
	}

	OpenVREventHandler::vr_event info = OpenVREventHandler::event_signals[event.eventType];
	Dictionary data;

	// The field names below are intentionally not cleaned up to remove type prefixes or
	// convert to snake case. There's so little documentation already that retaining
	// searchability is the least we can do. The upstream naming is also so inconsistent
	// that there's no predictable way to convert them anyway.
	switch (info.data_type) {
		case None:
			break;
		case Controller:
			data["button"] = event.data.controller.button;
			break;
		case Mouse:
			data["x"] = event.data.mouse.x;
			data["y"] = event.data.mouse.y;
			data["button"] = event.data.mouse.button;
			data["cursorIndex"] = event.data.mouse.cursorIndex;
			break;
		case Scroll:
			data["xdelta"] = event.data.scroll.xdelta;
			data["ydelta"] = event.data.scroll.ydelta;
			data["cursorIndex"] = event.data.scroll.cursorIndex;
			data["viewportscale"] = event.data.scroll.viewportscale;
			break;
		case Process:
			data["pid"] = event.data.process.pid;
			data["oldPid"] = event.data.process.oldPid;
			data["bForced"] = event.data.process.bForced;
			data["bConnectionLost"] = event.data.process.bConnectionLost;
			break;
		case Notification:
			data["ulUserValue"] = event.data.notification.ulUserValue;
			data["notificationId"] = event.data.notification.notificationId;
			break;
		case Overlay:
			data["cursorIndex"] = event.data.overlay.cursorIndex;
			data["devicePath"] = event.data.overlay.devicePath;
			data["overlayHandle"] = event.data.overlay.overlayHandle;
			data["memoryBlockId"] = event.data.overlay.memoryBlockId;
			break;
		case Status:
			data["statusState"] = event.data.status.statusState;
			break;
		case Keyboard:
			data["overlayHandle"] = event.data.keyboard.overlayHandle;
			data["cNewInput"] = event.data.keyboard.cNewInput;
			data["uUserValue"] = event.data.keyboard.uUserValue;
			break;
		case Ipd:
			data["ipdMeters"] = event.data.ipd.ipdMeters;
			break;
		case Chaperone:
			data["m_nCurrentUniverse"] = event.data.chaperone.m_nCurrentUniverse;
			data["m_nPreviousUniverse"] = event.data.chaperone.m_nPreviousUniverse;
			break;
		case PerformanceTest:
			data["m_nFidelityLevel"] = event.data.performanceTest.m_nFidelityLevel;
			break;
		case TouchPadMove:
			data["fValueXRaw"] = event.data.touchPadMove.fValueXRaw;
			data["fValueYRaw"] = event.data.touchPadMove.fValueYRaw;
			data["bFingerDown"] = event.data.touchPadMove.bFingerDown;
			data["fValueXFirst"] = event.data.touchPadMove.fValueXFirst;
			data["fValueYFirst"] = event.data.touchPadMove.fValueYFirst;
			data["flSecondsFingerDown"] = event.data.touchPadMove.flSecondsFingerDown;
			break;
		case SeatedZeroPoseReset:
			data["bResetBySystemMenu"] = event.data.seatedZeroPoseReset.bResetBySystemMenu;
			break;
		case Screenshot:
			data["type"] = event.data.screenshot.type;
			data["handle"] = event.data.screenshot.handle;
			break;
		case ScreenshotProgress:
			data["progress"] = event.data.screenshotProgress.progress;
			break;
		case ApplicationLaunch:
			data["pid"] = event.data.applicationLaunch.pid;
			data["unArgsHandle"] = event.data.applicationLaunch.unArgsHandle;
			break;
		case EditingCameraSurface:
			data["overlayHandle"] = event.data.cameraSurface.overlayHandle;
			data["nVisualMode"] = event.data.cameraSurface.nVisualMode;
			break;
		case MessageOverlay:
			data["unVRMessageOverlayResponse"] = event.data.messageOverlay.unVRMessageOverlayResponse;
			break;
		case Property:
			data["prop"] = event.data.property.prop;
			data["container"] = event.data.property.container;
			break;
		case HapticVibration:
			data["fAmplitude"] = event.data.hapticVibration.fAmplitude;
			data["fFrequency"] = event.data.hapticVibration.fFrequency;
			data["componentHandle"] = event.data.hapticVibration.componentHandle;
			data["containerHandle"] = event.data.hapticVibration.containerHandle;
			data["fDurationSeconds"] = event.data.hapticVibration.fDurationSeconds;
			break;
		case WebConsole:
			data["webConsoleHandle"] = event.data.webConsole.webConsoleHandle;
			break;
		case InputBindingLoad:
			data["pathUrl"] = event.data.inputBinding.pathUrl;
			data["pathMessage"] = event.data.inputBinding.pathMessage;
			data["ulAppContainer"] = event.data.inputBinding.ulAppContainer;
			data["pathControllerType"] = event.data.inputBinding.pathControllerType;
			break;
		case InputActionManifestLoad:
			data["pathMessage"] = event.data.actionManifest.pathMessage;
			data["pathAppKey"] = event.data.actionManifest.pathAppKey;
			data["pathManifestPath"] = event.data.actionManifest.pathManifestPath;
			data["pathMessageParam"] = event.data.actionManifest.pathMessageParam;
			break;
		case SpatialAnchor:
			data["unHandle"] = event.data.spatialAnchor.unHandle;
			break;
		case ProgressUpdate:
			data["pathIcon"] = event.data.progressUpdate.pathIcon;
			data["fProgress"] = event.data.progressUpdate.fProgress;
			data["pathDevice"] = event.data.progressUpdate.pathDevice;
			data["pathInputSource"] = event.data.progressUpdate.pathInputSource;
			data["pathProgressAction"] = event.data.progressUpdate.pathProgressAction;
			data["ulApplicationPropertyContainer"] = event.data.progressUpdate.ulApplicationPropertyContainer;
			break;
		case ShowUI:
			data["eType"] = event.data.showUi.eType;
			break;
		case ShowDevTools:
			data["nBrowserIdentifier"] = event.data.showDevTools.nBrowserIdentifier;
			break;
		case HDCPError:
			data["eCode"] = event.data.hdcpError.eCode;
			break;
		case AudioVolumeControl:
			data["fVolumeLevel"] = event.data.audioVolumeControl.fVolumeLevel;
			break;
		case AudioMuteControl:
			data["bMute"] = event.data.audioMuteControl.bMute;
			break;
	}

	this->emit_signal(info.signal_name, event.eventAgeSeconds, event.trackedDeviceIndex, data);
}

OpenVREventHandler::OpenVREventHandler() {
	ovr = openvr_data::retain_singleton();
	ovr->set_vrevent_handler(this);
}

OpenVREventHandler::~OpenVREventHandler() {
	ovr->remove_vrevent_handler(this);
	ovr->release();
}
