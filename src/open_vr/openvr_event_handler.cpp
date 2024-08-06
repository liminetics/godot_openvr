#include "openvr_event_handler.h"
#include <godot_cpp/variant/utility_functions.hpp>

#define VREVENT_SIGNAL(vrevent_name, vrevent_type)                                                                            \
	{                                                                                                                         \
		String name = String(#vrevent_name).trim_prefix("vr::EVREventType::VREvent_");                                        \
		ADD_SIGNAL(MethodInfo(name, PropertyInfo(Variant::DICTIONARY, "data")));                                              \
		OpenVREventHandler::event_signals.insert(vrevent_name, (vr_event){ .data_type = vrevent_type, .signal_name = name }); \
	}

using namespace godot;

VMap<uint32_t, OpenVREventHandler::vr_event> OpenVREventHandler::event_signals;

void OpenVREventHandler::_bind_methods() {
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceActivated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceDeactivated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUserInteractionStarted, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceUserInteractionEnded, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_IpdChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnterStandbyMode, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LeaveStandbyMode, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedDeviceRoleChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WatchdogWakeUpRequested, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LensDistortionChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PropertyChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WirelessDisconnect, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WirelessReconnect, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonPress, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonUnpress, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonTouch, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ButtonUntouch, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Modal_Cancel, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseMove, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseButtonDown, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MouseButtonUp, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FocusEnter, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FocusLeave, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScrollDiscrete, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TouchPadMove, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayFocusChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ReloadOverlays, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScrollSmooth, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LockMousePosition, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_UnlockMousePosition, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusCaptured, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusReleased, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InputFocusChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationUsingWrongGraphicsAdapter, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ActionBindingReloaded, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_HideRenderModels, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowRenderModels, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneApplicationStateChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SceneAppPipeDisconnected, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ConsoleOpened, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ConsoleClosed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayShown, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayHidden, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardActivated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardDeactivated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardRequested, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ResetDashboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ImageLoaded, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowKeyboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_HideKeyboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayGamepadFocusGained, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayGamepadFocusLost, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlaySharedTextureChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotTriggered, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ImageFailed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardOverlayCreated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SwitchGamepadFocus, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RequestScreenshot, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotTaken, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotFailed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SubmitScreenshotToDashboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ScreenshotProgressToDashboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PrimaryDashboardDeviceChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RoomViewShown, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RoomViewHidden, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowUI, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ShowDevTools, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopViewUpdating, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopViewReady, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_StartDashboard, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ElevatePrism, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayClosed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardThumbChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopMightBeVisible, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DesktopMightBeHidden, OpenVREventDataType::Reserved);
	// TODO: haven't updated openvr yet, these don't exist. Use them to test dynamic adding.
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_MutualSteamCapabilitiesChanged, OpenVREventDataType::Reserved);
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayCreated, OpenVREventDataType::Reserved);
	// VREVENT_SIGNAL(vr::EVREventType::VREvent_OverlayDestroyed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Shown, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Hidden, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_BeginInteraction, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Notification_Destroyed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Quit, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessQuit, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_QuitAcknowledged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DriverRequestedQuit, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_RestartRequested, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_InvalidateSwapTextureSets, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneDataHasChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneUniverseHasChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneTempDataHasChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SeatedZeroPoseReset, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneFlushCache, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneRoomSetupStarting, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ChaperoneRoomSetupFinished, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_StandingZeroPoseReset, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_AudioSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_BackgroundSettingHasChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_CameraSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ReprojectionSettingHasChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ModelSkinSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnvironmentSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PowerSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_EnableHomeAppSettingsHaveChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SteamVRSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LighthouseSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_NullSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_UserInterfaceSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_NotificationsSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerfSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DashboardSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WebInterfaceSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackersSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_LastKnownSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_DismissedWarningsSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_GpuSpeedSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WindowsMRSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_OtherSectionSettingChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_AnyDriverSettingsChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_StatusUpdate, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_WebInterface_InstallDriverCompleted, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MCImageUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FirmwareUpdateStarted, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_FirmwareUpdateFinished, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardClosed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardCharInput, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardDone, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardOpened_Global, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_KeyboardClosed_Global, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ApplicationListUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ApplicationMimeTypeLoad, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessConnected, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_ProcessDisconnected, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ChaperoneBoundsShown, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ChaperoneBoundsHidden, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayDisconnected, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayReconnected, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_HDCPError, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ApplicationNotResponding, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_ApplicationResumed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_OutOfVideoMemory, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_DisplayModeNotSupported, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_StageOverrideReady, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Compositor_RequestDisconnectReconnect, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_StartVideoStream, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_StopVideoStream, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_PauseVideoStream, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_ResumeVideoStream, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_TrackedCamera_EditingSurface, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_EnableCapture, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_DisableCapture, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_PerformanceTest_FidelityLevel, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MessageOverlay_Closed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_MessageOverlayCloseRequested, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_HapticVibration, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingLoadFailed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingLoadSuccessful, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ActionManifestReloaded, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ActionManifestLoadFailed, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_ProgressUpdate, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_TrackerActivated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingsUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Input_BindingSubscriptionChanged, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_PoseUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_DescriptorUpdated, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_RequestPoseUpdate, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SpatialAnchors_RequestDescriptorUpdate, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_SystemReport_Started, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Monitor_ShowHeadsetView, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Monitor_HideHeadsetView, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetSpeakersVolume, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetSpeakersMute, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetMicrophoneVolume, OpenVREventDataType::Reserved);
	VREVENT_SIGNAL(vr::EVREventType::VREvent_Audio_SetMicrophoneMute, OpenVREventDataType::Reserved);
}

void OpenVREventHandler::handle_event(vr::VREvent_t event) {
	if (!OpenVREventHandler::event_signals.has(event.eventType)) {
		return;
	}

	OpenVREventHandler::vr_event info = OpenVREventHandler::event_signals[event.eventType];
	Dictionary data;

	switch (info.data_type) {
		case Reserved:
		case Controller:
		case Mouse:
		case Scroll:
		case Process:
		case Notification:
		case Overlay:
		case Status:
		case Keyboard:
		case Ipd:
		case Chaperone:
		case PerformanceTest:
		case TouchPadMove:
		case SeatedZeroPoseReset:
		case Screenshot:
		case ScreenshotProgress:
		case ApplicationLaunch:
		case EditingCameraSurface:
		case MessageOverlay:
		case Property:
		case HapticVibration:
		case WebConsole:
		case InputBindingLoad:
		case InputActionManifestLoad:
		case SpatialAnchor:
		case ProgressUpdate:
		case ShowUI:
		case ShowDevTools:
		case HDCPError:
		case AudioVolumeControl:
		case AudioMuteControl: break;
	}

	this->emit_signal(info.signal_name, data);
}

OpenVREventHandler::OpenVREventHandler() {
	ovr = openvr_data::retain_singleton();
	ovr->set_vrevent_handler(this);
}

OpenVREventHandler::~OpenVREventHandler() {
	ovr->remove_vrevent_handler(this);
	ovr->release();
}
