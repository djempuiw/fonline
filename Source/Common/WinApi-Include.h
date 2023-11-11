//      __________        ___               ______            _
//     / ____/ __ \____  / (_)___  ___     / ____/___  ____ _(_)___  ___
//    / /_  / / / / __ \/ / / __ \/ _ \   / __/ / __ \/ __ `/ / __ \/ _ `
//   / __/ / /_/ / / / / / / / / /  __/  / /___/ / / / /_/ / / / / /  __/
//  /_/    \____/_/ /_/_/_/_/ /_/\___/  /_____/_/ /_/\__, /_/_/ /_/\___/
//                                                  /____/
// FOnline Engine
// https://fonline.ru
// https://github.com/cvet/fonline
//
// MIT License
//
// Copyright (c) 2006 - 2023, Anton Tsvetinskiy aka cvet <cvet@tut.by>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// WinApi universal header
#if FO_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Extra headers
#include "SDL_syswm.h"
#include <WinSock2.h>
#include <shellapi.h>

// Undef macro to avoid collisions
#undef min
#undef max
#undef AbnormalTermination
#undef AbortSystemShutdown
#undef AccessCheckAndAuditAlarm
#undef AccessCheckByTypeAndAuditAlarm
#undef AccessCheckByTypeResultListAndAuditAlarm
#undef AccessCheckByTypeResultListAndAuditAlarmByHandle
#undef AddAtom
#undef AddConsoleAlias
#undef AddFontResource
#undef AddFontResourceEx
#undef AnsiLower
#undef AnsiLowerBuff
#undef AnsiNext
#undef AnsiPrev
#undef AnsiToOem
#undef AnsiToOemBuff
#undef AnsiUpper
#undef AnsiUpperBuff
#undef AppendMenu
#undef BackupEventLog
#undef BeginUpdateResource
#undef BitScanForward
#undef BitScanForward64
#undef BitScanReverse
#undef BitScanReverse64
#undef BitTest
#undef BitTest64
#undef BitTestAndComplement
#undef BitTestAndComplement64
#undef BitTestAndReset
#undef BitTestAndReset64
#undef BitTestAndSet
#undef BitTestAndSet64
#undef BroadcastSystemMessage
#undef BroadcastSystemMessageEx
#undef BuildCommDCB
#undef BuildCommDCBAndTimeouts
#undef CacheLineFlush
#undef CallMsgFilter
#undef CallNamedPipe
#undef CallWindowProc
#undef CaptureStackBackTrace
#undef ChangeDisplaySettings
#undef ChangeDisplaySettingsEx
#undef ChangeMenu
#undef ChangeServiceConfig
#undef ChangeServiceConfig2
#undef CharLower
#undef CharLowerBuff
#undef CharNext
#undef CharPrev
#undef CharToOem
#undef CharToOemBuff
#undef CharUpper
#undef CharUpperBuff
#undef CheckNameLegalDOS8Dot3
#undef ClearEventLog
#undef CommConfigDialog
#undef CompareString
#undef ControlServiceEx
#undef CopyAcceleratorTable
#undef CopyCursor
#undef CopyEnhMetaFile
#undef CopyFile
#undef CopyFileEx
#undef CopyFileTransacted
#undef CopyMemory
#undef CopyMetaFile
#undef CpuIdEx
#undef CreateAcceleratorTable
#undef CreateActCtx
#undef CreateBoundaryDescriptor
#undef CreateColorSpace
#undef CreateDC
#undef CreateDesktop
#undef CreateDesktopEx
#undef CreateDialog
#undef CreateDialogA
#undef CreateDialogIndirect
#undef CreateDialogIndirectA
#undef CreateDialogIndirectParam
#undef CreateDialogIndirectW
#undef CreateDialogParam
#undef CreateDialogW
#undef CreateDirectory
#undef CreateDirectoryEx
#undef CreateDirectoryTransacted
#undef CreateEnhMetaFile
#undef CreateEvent
#undef CreateEventEx
#undef CreateFile
#undef CreateFileMapping
#undef CreateFileMappingNuma
#undef CreateFileTransacted
#undef CreateFont
#undef CreateFontIndirect
#undef CreateFontIndirectEx
#undef CreateHardLink
#undef CreateHardLinkTransacted
#undef CreateIC
#undef CreateJobObject
#undef CreateMDIWindow
#undef CreateMailslot
#undef CreateMetaFile
#undef CreateMutex
#undef CreateMutexEx
#undef CreateNamedPipe
#undef CreatePrivateNamespace
#undef CreateProcess
#undef CreateProcessAsUser
#undef CreateScalableFontResource
#undef CreateSemaphore
#undef CreateSemaphoreEx
#undef CreateService
#undef CreateSymbolicLink
#undef CreateSymbolicLinkTransacted
#undef CreateWaitableTimer
#undef CreateWaitableTimerEx
#undef CreateWindow
#undef CreateWindowA
#undef CreateWindowEx
#undef CreateWindowStation
#undef CreateWindowW
#undef DbgRaiseAssertionFailure
#undef DecryptFile
#undef DefDlgProc
#undef DefFrameProc
#undef DefHookProc
#undef DefMDIChildProc
#undef DefWindowProc
#undef DefineDosDevice
#undef DefineHandleTable
#undef DeleteFile
#undef DeleteFileTransacted
#undef DeleteVolumeMountPoint
#undef DeviceCapabilities
#undef DialogBox
#undef DialogBoxA
#undef DialogBoxIndirect
#undef DialogBoxIndirectA
#undef DialogBoxIndirectParam
#undef DialogBoxIndirectW
#undef DialogBoxParam
#undef DialogBoxW
#undef DispatchMessage
#undef DlgDirList
#undef DlgDirListComboBox
#undef DlgDirSelectComboBoxEx
#undef DlgDirSelectEx
#undef DnsHostnameToComputerName
#undef DrawState
#undef DrawText
#undef DrawTextEx
#undef EncryptFile
#undef EndUpdateResource
#undef EnumCalendarInfo
#undef EnumCalendarInfoEx
#undef EnumDateFormats
#undef EnumDateFormatsEx
#undef EnumDependentServices
#undef EnumDesktops
#undef EnumDisplayDevices
#undef EnumDisplaySettings
#undef EnumDisplaySettingsEx
#undef EnumFontFamilies
#undef EnumFontFamiliesEx
#undef EnumFonts
#undef EnumICMProfiles
#undef EnumLanguageGroupLocales
#undef EnumProps
#undef EnumPropsEx
#undef EnumResourceLanguages
#undef EnumResourceLanguagesEx
#undef EnumResourceNames
#undef EnumResourceNamesEx
#undef EnumResourceTypes
#undef EnumResourceTypesEx
#undef EnumServicesStatus
#undef EnumServicesStatusEx
#undef EnumSystemCodePages
#undef EnumSystemLanguageGroups
#undef EnumSystemLocales
#undef EnumTaskWindows
#undef EnumTimeFormats
#undef EnumUILanguages
#undef EnumWindowStations
#undef ExitWindows
#undef ExpandEnvironmentStrings
#undef ExpungeConsoleCommandHistory
#undef ExtTextOut
#undef FastFence
#undef FatalAppExit
#undef FileEncryptionStatus
#undef FillConsoleOutputCharacter
#undef FillMemory
#undef FindActCtxSectionString
#undef FindAtom
#undef FindFirstChangeNotification
#undef FindFirstFile
#undef FindFirstFileEx
#undef FindFirstFileTransacted
#undef FindFirstVolume
#undef FindFirstVolumeMountPoint
#undef FindNextFile
#undef FindNextVolume
#undef FindNextVolumeMountPoint
#undef FindResource
#undef FindResourceEx
#undef FindWindow
#undef FindWindowEx
#undef FoldString
#undef FormatMessage
#undef FreeEnvironmentStrings
#undef FreeModule
#undef FreeProcInstance
#undef GetAltTabInfo
#undef GetAtomName
#undef GetBValue
#undef GetBinaryType
#undef GetCPInfoEx
#undef GetCValue
#undef GetCalendarInfo
#undef GetCallersEflags
#undef GetCharABCWidths
#undef GetCharABCWidthsFloat
#undef GetCharWidth
#undef GetCharWidth32
#undef GetCharWidthFloat
#undef GetCharacterPlacement
#undef GetClassInfo
#undef GetClassInfoEx
#undef GetClassLong
#undef GetClassLongPtr
#undef GetClassName
#undef GetClipboardFormatName
#undef GetCommandLine
#undef GetCompressedFileSize
#undef GetCompressedFileSizeTransacted
#undef GetComputerName
#undef GetComputerNameEx
#undef GetConsoleAlias
#undef GetConsoleAliasExes
#undef GetConsoleAliasExesLength
#undef GetConsoleAliases
#undef GetConsoleAliasesLength
#undef GetConsoleCommandHistory
#undef GetConsoleCommandHistoryLength
#undef GetConsoleOriginalTitle
#undef GetConsoleTitle
#undef GetCurrencyFormat
#undef GetCurrentDirectory
#undef GetCurrentHwProfile
#undef GetCurrentTime
#undef GetDateFormat
#undef GetDefaultCommConfig
#undef GetDiskFreeSpace
#undef GetDiskFreeSpaceEx
#undef GetDiskSpaceInformation
#undef GetDlgItemText
#undef GetDllDirectory
#undef GetDriveType
#undef GetEnhMetaFile
#undef GetEnhMetaFileDescription
#undef GetEnvironmentStringsA
#undef GetEnvironmentVariable
#undef GetExceptionCode
#undef GetExceptionInformation
#undef GetFileAttributes
#undef GetFileAttributesEx
#undef GetFileAttributesTransacted
#undef GetFileSecurity
#undef GetFileVersionInfo
#undef GetFileVersionInfoEx
#undef GetFileVersionInfoSize
#undef GetFileVersionInfoSizeEx
#undef GetFinalPathNameByHandle
#undef GetFirmwareEnvironmentVariable
#undef GetFirmwareEnvironmentVariableEx
#undef GetFreeSpace
#undef GetFullPathName
#undef GetFullPathNameTransacted
#undef GetGValue
#undef GetGeoInfo
#undef GetGlyphIndices
#undef GetGlyphOutline
#undef GetICMProfile
#undef GetIconInfoEx
#undef GetKValue
#undef GetKerningPairs
#undef GetKeyNameText
#undef GetKeyboardLayoutName
#undef GetLocaleInfo
#undef GetLogColorSpace
#undef GetLogicalDriveStrings
#undef GetLongPathName
#undef GetLongPathNameTransacted
#undef GetMValue
#undef GetMenuItemInfo
#undef GetMenuString
#undef GetMessage
#undef GetMetaFile
#undef GetModuleFileName
#undef GetModuleHandle
#undef GetModuleHandleEx
#undef GetMonitorInfo
#undef GetNamedPipeClientComputerName
#undef GetNamedPipeHandleState
#undef GetNextWindow
#undef GetNumberFormat
#undef GetObject
#undef GetOutlineTextMetrics
#undef GetPrivateProfileInt
#undef GetPrivateProfileSection
#undef GetPrivateProfileSectionNames
#undef GetPrivateProfileString
#undef GetPrivateProfileStruct
#undef GetProfileInt
#undef GetProfileSection
#undef GetProfileString
#undef GetProp
#undef GetRValue
#undef GetRawInputDeviceInfo
#undef GetScode
#undef GetSegmentLimit
#undef GetServiceDisplayName
#undef GetServiceKeyName
#undef GetShortPathName
#undef GetStartupInfo
#undef GetStringTypeEx
#undef GetSysModalWindow
#undef GetSystemDirectory
#undef GetSystemWindowsDirectory
#undef GetSystemWow64Directory
#undef GetSystemWow64Directory2
#undef GetTabbedTextExtent
#undef GetTempFileName
#undef GetTempPath
#undef GetTextExtentExPoint
#undef GetTextExtentPoint
#undef GetTextExtentPoint32
#undef GetTextFace
#undef GetTextMetrics
#undef GetTimeFormat
#undef GetUserName
#undef GetUserObjectInformation
#undef GetVersionEx
#undef GetVolumeInformation
#undef GetVolumeNameForVolumeMountPoint
#undef GetVolumePathName
#undef GetVolumePathNamesForVolumeName
#undef GetWindowLong
#undef GetWindowLongPtr
#undef GetWindowModuleFileName
#undef GetWindowTask
#undef GetWindowText
#undef GetWindowTextLength
#undef GetWindowsDirectory
#undef GetYValue
#undef GlobalAddAtom
#undef GlobalAddAtomEx
#undef GlobalDiscard
#undef GlobalFindAtom
#undef GlobalGetAtomName
#undef GlobalLRUNewest
#undef GlobalLRUOldest
#undef GrayString
#undef Handle64ToHandle
#undef HandleToHandle32
#undef HandleToHandle64
#undef HandleToUlong
#undef HasOverlappedIoCompleted
#undef ImmConfigureIME
#undef ImmEnumRegisterWord
#undef ImmEscape
#undef ImmGetCandidateList
#undef ImmGetCandidateListCount
#undef ImmGetCompositionFont
#undef ImmGetCompositionString
#undef ImmGetConversionList
#undef ImmGetDescription
#undef ImmGetGuideLine
#undef ImmGetIMEFileName
#undef ImmGetImeMenuItems
#undef ImmGetRegisterWordStyle
#undef ImmInstallIME
#undef ImmIsUIMessage
#undef ImmRegisterWord
#undef ImmSetCompositionFont
#undef ImmSetCompositionString
#undef ImmUnregisterWord
#undef InitiateShutdown
#undef InitiateSystemShutdown
#undef InitiateSystemShutdownEx
#undef InsertMenu
#undef InsertMenuItem
#undef Int32x32To64
#undef Int64ShllMod32
#undef Int64ShraMod32
#undef Int64ShrlMod32
#undef InterlockedAdd
#undef InterlockedAdd64
#undef InterlockedAddAcquire
#undef InterlockedAddAcquire64
#undef InterlockedAddNoFence
#undef InterlockedAddNoFence64
#undef InterlockedAddRelease
#undef InterlockedAddRelease64
#undef InterlockedAnd
#undef InterlockedAnd16
#undef InterlockedAnd64
#undef InterlockedAnd64Acquire
#undef InterlockedAnd64NoFence
#undef InterlockedAnd64Release
#undef InterlockedAnd8
#undef InterlockedAndAcquire
#undef InterlockedAndAffinity
#undef InterlockedAndNoFence
#undef InterlockedAndRelease
#undef InterlockedBitTestAndReset
#undef InterlockedBitTestAndReset64
#undef InterlockedBitTestAndReset64Acquire
#undef InterlockedBitTestAndReset64NoFence
#undef InterlockedBitTestAndReset64Release
#undef InterlockedBitTestAndResetAcquire
#undef InterlockedBitTestAndResetNoFence
#undef InterlockedBitTestAndResetRelease
#undef InterlockedBitTestAndSet
#undef InterlockedBitTestAndSet64
#undef InterlockedBitTestAndSet64Acquire
#undef InterlockedBitTestAndSet64NoFence
#undef InterlockedBitTestAndSet64Release
#undef InterlockedBitTestAndSetAcquire
#undef InterlockedBitTestAndSetNoFence
#undef InterlockedBitTestAndSetRelease
#undef InterlockedCompareExchange
#undef InterlockedCompareExchange128
#undef InterlockedCompareExchange16
#undef InterlockedCompareExchange64
#undef InterlockedCompareExchangeAcquire
#undef InterlockedCompareExchangeAcquire16
#undef InterlockedCompareExchangeAcquire64
#undef InterlockedCompareExchangeNoFence
#undef InterlockedCompareExchangeNoFence16
#undef InterlockedCompareExchangeNoFence64
#undef InterlockedCompareExchangePointer
#undef InterlockedCompareExchangePointerAcquire
#undef InterlockedCompareExchangePointerNoFence
#undef InterlockedCompareExchangePointerRelease
#undef InterlockedCompareExchangeRelease
#undef InterlockedCompareExchangeRelease16
#undef InterlockedCompareExchangeRelease64
#undef InterlockedDecrement
#undef InterlockedDecrement16
#undef InterlockedDecrement64
#undef InterlockedDecrementAcquire
#undef InterlockedDecrementAcquire16
#undef InterlockedDecrementAcquire64
#undef InterlockedDecrementNoFence
#undef InterlockedDecrementNoFence16
#undef InterlockedDecrementNoFence64
#undef InterlockedDecrementRelease
#undef InterlockedDecrementRelease16
#undef InterlockedDecrementRelease64
#undef InterlockedDecrementSizeT
#undef InterlockedDecrementSizeTNoFence
#undef InterlockedExchange
#undef InterlockedExchange16
#undef InterlockedExchange64
#undef InterlockedExchange8
#undef InterlockedExchangeAcquire
#undef InterlockedExchangeAcquire64
#undef InterlockedExchangeAdd
#undef InterlockedExchangeAdd64
#undef InterlockedExchangeAdd8
#undef InterlockedExchangeAddAcquire
#undef InterlockedExchangeAddAcquire64
#undef InterlockedExchangeAddNoFence
#undef InterlockedExchangeAddNoFence64
#undef InterlockedExchangeAddRelease
#undef InterlockedExchangeAddRelease64
#undef InterlockedExchangeAddSizeT
#undef InterlockedExchangeAddSizeTAcquire
#undef InterlockedExchangeAddSizeTNoFence
#undef InterlockedExchangeNoFence
#undef InterlockedExchangeNoFence64
#undef InterlockedExchangePointer
#undef InterlockedExchangePointerAcquire
#undef InterlockedExchangePointerNoFence
#undef InterlockedIncrement
#undef InterlockedIncrement16
#undef InterlockedIncrement64
#undef InterlockedIncrementAcquire
#undef InterlockedIncrementAcquire16
#undef InterlockedIncrementAcquire64
#undef InterlockedIncrementNoFence
#undef InterlockedIncrementNoFence16
#undef InterlockedIncrementNoFence64
#undef InterlockedIncrementRelease
#undef InterlockedIncrementRelease16
#undef InterlockedIncrementRelease64
#undef InterlockedIncrementSizeT
#undef InterlockedIncrementSizeTNoFence
#undef InterlockedOr
#undef InterlockedOr16
#undef InterlockedOr64
#undef InterlockedOr64Acquire
#undef InterlockedOr64NoFence
#undef InterlockedOr64Release
#undef InterlockedOr8
#undef InterlockedOrAcquire
#undef InterlockedOrAffinity
#undef InterlockedOrNoFence
#undef InterlockedOrRelease
#undef InterlockedPushListSList
#undef InterlockedXor
#undef InterlockedXor16
#undef InterlockedXor64
#undef InterlockedXor64Acquire
#undef InterlockedXor64NoFence
#undef InterlockedXor64Release
#undef InterlockedXor8
#undef InterlockedXorAcquire
#undef InterlockedXorNoFence
#undef InterlockedXorRelease
#undef IsBadStringPtr
#undef IsCharAlpha
#undef IsCharAlphaNumeric
#undef IsCharLower
#undef IsCharUpper
#undef IsDialogMessage
#undef IsEqualCLSID
#undef IsEqualFMTID
#undef IsEqualIID
#undef IsReparseTagDirectory
#undef IsReparseTagMicrosoft
#undef IsReparseTagNameSurrogate
#undef IsVirtualDiskFileShared
#undef LCMapString
#undef LPTx
#undef LimitEmsPages
#undef LoadAccelerators
#undef LoadBitmap
#undef LoadCursor
#undef LoadCursorFromFile
#undef LoadEnclaveImage
#undef LoadFence
#undef LoadIcon
#undef LoadImage
#undef LoadKeyboardLayout
#undef LoadLibrary
#undef LoadLibraryEx
#undef LoadMenu
#undef LoadMenuIndirect
#undef LoadString
#undef LocalDiscard
#undef LockSegment
#undef LogonUser
#undef LogonUserEx
#undef LookupAccountName
#undef LookupAccountNameLocal
#undef LookupAccountSid
#undef LookupAccountSidLocal
#undef LookupPrivilegeDisplayName
#undef LookupPrivilegeName
#undef LookupPrivilegeValue
#undef MakeProcInstance
#undef MapVirtualKey
#undef MapVirtualKeyEx
#undef MemoryBarrier
#undef MemoryFence
#undef MessageBox
#undef MessageBoxEx
#undef MessageBoxIndirect
#undef ModifyMenu
#undef MoveFile
#undef MoveFileEx
#undef MoveFileTransacted
#undef MoveFileWithProgress
#undef MoveMemory
#undef MultinetGetConnectionPerformance
#undef Multiply128
#undef MultiplyHigh
#undef NeedCurrentDirectoryForExePath
#undef NotifyServiceStatusChange
#undef ObjectCloseAuditAlarm
#undef ObjectDeleteAuditAlarm
#undef ObjectOpenAuditAlarm
#undef ObjectPrivilegeAuditAlarm
#undef OemToAnsi
#undef OemToAnsiBuff
#undef OemToChar
#undef OemToCharBuff
#undef OpenBackupEventLog
#undef OpenDesktop
#undef OpenEncryptedFileRaw
#undef OpenEvent
#undef OpenEventLog
#undef OpenFileMapping
#undef OpenJobObject
#undef OpenMutex
#undef OpenPrivateNamespace
#undef OpenSCManager
#undef OpenSemaphore
#undef OpenService
#undef OpenWaitableTimer
#undef OpenWindowStation
#undef OutputDebugString
#undef PeekConsoleInput
#undef PeekMessage
#undef PolyTextOut
#undef PopulationCount64
#undef PostAppMessage
#undef PostAppMessageA
#undef PostAppMessageW
#undef PostMessage
#undef PostThreadMessage
#undef PreFetchCacheLine
#undef PrefetchForWrite
#undef PrivateExtractIcons
#undef PrivilegedServiceAuditAlarm
#undef ProcThreadAttributeValue
#undef PropagateResult
#undef Ptr64ToPtr
#undef PtrToPtr64
#undef QueryDosDevice
#undef QueryFullProcessImageName
#undef QueryServiceConfig
#undef QueryServiceConfig2
#undef QueryServiceLockStatus
#undef ReadConsole
#undef ReadConsoleInput
#undef ReadConsoleOutput
#undef ReadConsoleOutputCharacter
#undef ReadEventLog
#undef ReadForWriteAccess
#undef ReadLongPtrAcquire
#undef ReadLongPtrNoFence
#undef ReadLongPtrRaw
#undef ReadMxCsr
#undef ReadPMC
#undef ReadSizeTAcquire
#undef ReadSizeTNoFence
#undef ReadSizeTRaw
#undef ReadTimeStampCounter
#undef ReadULongPtrAcquire
#undef ReadULongPtrNoFence
#undef ReadULongPtrRaw
#undef RealGetWindowClass
#undef RegConnectRegistry
#undef RegConnectRegistryEx
#undef RegCopyTree
#undef RegCreateKey
#undef RegCreateKeyEx
#undef RegCreateKeyTransacted
#undef RegDeleteKey
#undef RegDeleteKeyEx
#undef RegDeleteKeyTransacted
#undef RegDeleteKeyValue
#undef RegDeleteTree
#undef RegDeleteValue
#undef RegEnumKey
#undef RegEnumKeyEx
#undef RegEnumValue
#undef RegGetValue
#undef RegLoadAppKey
#undef RegLoadKey
#undef RegLoadMUIString
#undef RegOpenKey
#undef RegOpenKeyEx
#undef RegOpenKeyTransacted
#undef RegQueryInfoKey
#undef RegQueryMultipleValues
#undef RegQueryValue
#undef RegQueryValueEx
#undef RegReplaceKey
#undef RegRestoreKey
#undef RegSaveKey
#undef RegSaveKeyEx
#undef RegSetKeyValue
#undef RegSetValue
#undef RegSetValueEx
#undef RegUnLoadKey
#undef RegisterClass
#undef RegisterClassEx
#undef RegisterClipboardFormat
#undef RegisterDeviceNotification
#undef RegisterEventSource
#undef RegisterServiceCtrlHandler
#undef RegisterServiceCtrlHandlerEx
#undef RegisterWindowMessage
#undef RemoveDirectory
#undef RemoveDirectoryTransacted
#undef RemoveFontResource
#undef RemoveFontResourceEx
#undef RemoveProp
#undef ReplaceFile
#undef ReportEvent
#undef ResetDC
#undef ResultFromScode
#undef RotateLeft16
#undef RotateLeft32
#undef RotateLeft64
#undef RotateLeft8
#undef RotateRight16
#undef RotateRight32
#undef RotateRight64
#undef RotateRight8
#undef RtlCopyMemory
#undef RtlEqualMemory
#undef RtlFillMemory
#undef RtlMoveMemory
#undef RtlZeroMemory
#undef ScrollConsoleScreenBuffer
#undef SearchPath
#undef SecureZeroMemory
#undef SendDlgItemMessage
#undef SendMessage
#undef SendMessageCallback
#undef SendMessageTimeout
#undef SendNotifyMessage
#undef SetCalendarInfo
#undef SetClassLong
#undef SetClassLongPtr
#undef SetComputerName
#undef SetComputerNameEx
#undef SetConsoleNumberOfCommands
#undef SetConsoleTitle
#undef SetCurrentDirectory
#undef SetDefaultCommConfig
#undef SetDlgItemText
#undef SetDllDirectory
#undef SetEnvironmentStrings
#undef SetEnvironmentVariable
#undef SetFileAttributes
#undef SetFileAttributesTransacted
#undef SetFileSecurity
#undef SetFileShortName
#undef SetFirmwareEnvironmentVariable
#undef SetFirmwareEnvironmentVariableEx
#undef SetICMProfile
#undef SetLocaleInfo
#undef SetMenuItemInfo
#undef SetProp
#undef SetSwapAreaSize
#undef SetSysModalWindow
#undef SetUserObjectInformation
#undef SetVolumeLabel
#undef SetVolumeMountPoint
#undef SetWindowLong
#undef SetWindowLongPtr
#undef SetWindowText
#undef SetWindowsHook
#undef SetWindowsHookEx
#undef ShiftLeft128
#undef ShiftRight128
#undef SpeculationFence
#undef StartDoc
#undef StartService
#undef StartServiceCtrlDispatcher
#undef StoreFence
#undef SystemParametersInfo
#undef TabbedTextOut
#undef TextOut
#undef TranslateAccelerator
#undef UInt32x32To64
#undef UintToPtr
#undef UlongToHandle
#undef UlongToPtr
#undef UnlockResource
#undef UnlockSegment
#undef UnregisterClass
#undef UnsignedMultiply128
#undef UnsignedMultiplyHigh
#undef UpdateICMRegKey
#undef UpdateResource
#undef VerFindFile
#undef VerInstallFile
#undef VerLanguageName
#undef VerQueryValue
#undef VerifyVersionInfo
#undef VkKeyScan
#undef VkKeyScanEx
#undef WNetAddConnection
#undef WNetAddConnection2
#undef WNetAddConnection3
#undef WNetCancelConnection
#undef WNetCancelConnection2
#undef WNetConnectionDialog1
#undef WNetDisconnectDialog1
#undef WNetEnumResource
#undef WNetGetConnection
#undef WNetGetLastError
#undef WNetGetNetworkInformation
#undef WNetGetProviderName
#undef WNetGetResourceInformation
#undef WNetGetResourceParent
#undef WNetGetUniversalName
#undef WNetGetUser
#undef WNetOpenEnum
#undef WNetUseConnection
#undef WaitNamedPipe
#undef WinHelp
#undef WriteConsole
#undef WriteConsoleInput
#undef WriteConsoleOutput
#undef WriteConsoleOutputCharacter
#undef WriteLongPtrNoFence
#undef WriteLongPtrRaw
#undef WriteLongPtrRelease
#undef WriteMxCsr
#undef WritePrivateProfileSection
#undef WritePrivateProfileString
#undef WritePrivateProfileStruct
#undef WriteProfileSection
#undef WriteProfileString
#undef WriteSizeTNoFence
#undef WriteSizeTRaw
#undef WriteSizeTRelease
#undef WriteULongPtrNoFence
#undef WriteULongPtrRaw
#undef WriteULongPtrRelease
#undef Yield
#undef YieldProcessor
#undef ZeroMemory
#undef AddJob
#endif
