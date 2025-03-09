// ///////////////////////////////////////////////////////////////////////////
//
//	@brief API interface of Enrigin Managerment Library
//	Erml API Binding for Go
//	Enrigin Tech, All Rights Reserved. 2024 Copyright (C)
//
// ///////////////////////////////////////////////////////////////////////////
package erml

// #cgo LDFLAGS: -ldl  -Wl,--unresolved-symbols=ignore-in-object-files
// #include "stdbool.h"
// #include "erml.h"
import "C"

import (
	"bufio"
	"fmt"
	"os"
	"os/signal"
	"strconv"
	"strings"
	"syscall"
)

type Handle struct {
	Dev_Idx uint
}

const (
	szName = C.MAX_CHAR_BUFF_LEN
	szUUID = C.MAX_CHAR_BUFF_LEN
)

type DevThermalInfo struct {
	Cur_Dev_Temp  float32
	Cur_Hbm0_Temp float32
	Cur_Hbm1_Temp float32
}

type DevThermalInfoV2 struct {
	Cur_Asic_Temp  float32
	Cur_Mem_Temp   float32
	Cur_Board_Temp float32
}

type DevPowerInfo struct {
	Pwr_Capability      float32
	Cur_Pwr_Consumption float32
}

type DevMemInfo struct {
	Mem_Total_Size uint
	Mem_Used       uint
}

type ClusterHbmMemInfo struct {
	Mem_Total_Size uint
	Mem_Used       uint
}

type DevClkInfo struct {
	Cur_Hbm_Clock uint
	Cur_Dtu_Clock uint
}

type DeviceInfo struct {
	Name      string
	Vendor_Id uint
	Device_Id uint
	Domain_Id uint
	Bus_Id    uint
	Dev_Id    uint
	Func_Id   uint
}

type EventInfo struct {
	Id   uint
	Type uint
	Msg  string
}

type LinkInfo struct {
	Link_Speed     uint
	Max_Link_Speed uint
	Link_Width     uint
	Max_Link_Width uint
}

type ThroughputInfo struct {
	Tx_Throughput float32
	Rx_Throughput float32
	Tx_Nak        uint64
	Rx_Nak        uint64
}

type EslPortInfo struct {
	Connected        uint
	Uuid             string
	Vendor_Id        uint
	Device_Id        uint
	Domain_Id        uint
	Bus_Id           uint
	Dev_Id           uint
	Func_Id          uint
	Port_Id          uint
	Port_Type        uint
	Remote_Card_Id   uint
	Remote_Uuid      string
	Remote_Vendor_Id uint
	Remote_Device_Id uint
	Remote_Domain_Id uint
	Remote_Bus_Id    uint
	Remote_Dev_Id    uint
	Remote_Func_Id   uint
	Remote_Port_Id   uint
	Remote_Port_Type uint
}

type DevEccStatus struct {
	Enabled bool
	Pending bool
	Pdblack bool
	Ecnt_sb uint
	Ecnt_db uint
}

type DevRmaStatus struct {
	SupportRma bool
	Flags      bool
}

type DevRmaDetails struct {
	SupportRma bool
	Flags      bool
	Dbe        uint
}

type ProcessInfo struct {
	Pid         uint
	DevMemUsage uint64
	SysMemUsage uint64
}

type HwArch int

const (
	GCU200  HwArch = 0
	GCU210  HwArch = 1
	GCU300  HwArch = 2
	GCU310  HwArch = 3
	Unknown HwArch = 65535
)

type ErmlError struct {
	ErrCode int
	Msg     string
}

func (e ErmlError) Error() string {
	return e.Msg
}
func (e ErmlError) Code() int {
	return e.ErrCode
}

var (
	ErrSuccess             = ErmlError{int(C.ERML_SUCCESS), "No error"}
	ErrUnInit              = ErmlError{int(C.ERML_ERROR_UNINITIALIZED), "Error, device un-initialized"}
	ErrInvalidArg          = ErmlError{int(C.ERML_ERROR_INVALID_ARGUMENT), "Error, invalid argument"}
	ErrUnSupport           = ErmlError{int(C.ERML_ERROR_NOT_SUPPORTED), "Error, not supported operation"}
	ErrLibNotFound         = ErmlError{int(C.ERML_ERROR_LIBRARY_NOT_FOUND), "Error, library not found"}
	ErrInvalidErrCode      = ErmlError{int(C.ERML_ERROR_INVALID_ERROR_CODE), "Error, invalid error code"}
	ErrDriverNotLoad       = ErmlError{int(C.ERML_ERROR_DRIVER_NOT_LOADED), "Error, driver not loaded"}
	ErrEslPortNum          = ErmlError{int(C.ERML_ERROR_ESL_PORT_NUMBER_ERR), "Error, esl port number error"}
	ErrInvalidInput        = ErmlError{int(C.ERML_ERROR_INVALID_INPUT), "Error, invalid input"}
	ErrFuncNotFound        = ErmlError{int(C.ERML_ERROR_FUNCTION_NOT_FOUND), "Error, func not found"}
	ErrFailedOpenDriver    = ErmlError{int(C.ERML_ERROR_OPEN_DRIVER_VERSION), "Error, failed to open driver version"}
	ErrDriverNotCompatible = ErmlError{int(C.ERML_ERROR_DRIVER_NOT_COMPATIBLE), "Error, driver version is incompatible"}
	ErrNoDevice            = ErmlError{int(C.ERML_ERROR_NO_DEVICE), "Error, no supported device"}
	ErrTimeout             = ErmlError{int(C.ERML_ERROR_TIMEOUT), "Error, timeout"}
	ErrOpFail              = ErmlError{int(C.ERML_ERROR_FAIL), "Error, operate fail"}
	ErrMax                 = ErmlError{int(C.ERML_ERROR_MAX), "Error, this is the max error code"}
)

// utils function
func uintPtr(c C.uint) *uint {
	i := uint(c)
	return &i
}

func uint64Ptr(c C.uint64_t) *uint64 {
	i := uint64(c)
	return &i
}

func floatPtr(c C.float) *float32 {
	i := float32(c)
	return &i
}

func stringPtr(c *C.char) *string {
	s := C.GoString(c)
	return &s
}

func errorString(ret C.ermlReturn_t) error {
	var cerr [szName]C.char

	if ret == C.ERML_SUCCESS {
		return nil
	}

	C.ErmlErrorString(ret, &cerr[0])
	err := C.GoString(&cerr[0])

	return ErmlError{int(ret), err}
}

func (h Handle) GetLogicId() (uint, error) {
	var logic_id C.uint
	r := C.ErmlGetDevLogicId(C.uint(h.Dev_Idx), &logic_id)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		fmt.Println("can't find dev logic node:", errorString(r).Error())
		return 0, errorString(r)
	}
	return uint(logic_id), nil
}

func (h Handle) GetBusId() (path string, err error) {

	path = ""
	devInfo, err := h.GetDevInfo()
	if err != nil {
		return
	}
	driverAP, err := GetDriverAccessPoint()

	if err == nil {
		path = fmt.Sprintf("%s%04x:%02x:%02x.%x", driverAP, devInfo.Domain_Id, devInfo.Bus_Id, devInfo.Dev_Id, devInfo.Func_Id)
	}
	return
}

/*
 * @brief Enrigin Management Library Initialization.
 * @return ermlReturn_t
 */
func Init() error {
	// SIGUSR1 for PSE erml have special use
	signal.Ignore(syscall.SIGUSR1)
	signal.Ignore(syscall.SIGUSR2)
	noDriver := false
	return errorString(dl.InitV2(noDriver))
}

/*
 * @brief Enrigin Management Library Initialization.
 * @return ermlReturn_t
 */
func InitV2(no_driver /* use driver or not */ bool) error {
	// SIGUSR1 for PSE erml have special use
	signal.Ignore(syscall.SIGUSR1)
	signal.Ignore(syscall.SIGUSR2)
	return errorString(dl.InitV2(no_driver))
}

/*
 * @brief Enrigin Management Library Shutdown.
 */
func Shutdown() error {
	return errorString(dl.Shutdown())
}

/*
 * @brief Enrigin Management Library get driver version info.
 */
func GetDriverVer() (string, error) {
	var ver [szName]C.char

	r := C.ErmlGetDriverVer(&ver[0])
	return C.GoString(&ver[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get driver config and status access path info.
 */
func GetDriverAccessPoint() (string, error) {
	var ver [szName]C.char

	r := C.ErmlGetDriverAccessPoint(&ver[0])
	return C.GoString(&ver[0]), errorString(r)
}

/**
 * @brief Enrigin Management Library get the total number of special device's clusters.
 *
 */
func getClusterCount_v1(dev_idx uint) (uint, error) {
	var cluster_cnt C.uint
	r := C.ErmlGetClusterCount(C.uint(dev_idx), &cluster_cnt)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(cluster_cnt), errorString(r)
}

func (h Handle) GetClusterCount() (uint, error) {
	return getClusterCount_v1(h.Dev_Idx)
}

/*
 * @brief Enrigin Management Library get the total number of supported devices.
 */
func getDevCount_v1() (uint, error) {
	var dev_cnt C.uint
	r := C.ErmlGetDevCount(&dev_cnt)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(dev_cnt), errorString(r)
}

func GetDevCountFromErml() (uint, error) {
	dev_cnt, err := getDevCount_v1()
	return uint(dev_cnt), err
}

func GetDevCount() (uint, error) {
	var dev_cnt C.uint
	r := C.ErmlGetDevCount(&dev_cnt)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(dev_cnt), errorString(r)
}

/*
 * @brief Enrigin Management Library get the device name.
 */
func (h Handle) GetDevName() (string /* device_Name */, error) {
	var name [szName]C.char

	r := C.ErmlGetDevName(C.uint(h.Dev_Idx), &name[0])
	return C.GoString(&name[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get the slot or OAM name.
 */
func (h Handle) GetDevSlotOamName() (string /* slot_Name */, error) {
	var name [szName]C.char

	r := C.ErmlGetDevSlotOamName(C.uint(h.Dev_Idx), &name[0])
	return C.GoString(&name[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get the device temperature.
 */
func (h Handle) GetDevTemp() (thermalInfo *DevThermalInfo, err error) {
	var thermal C.ermlDevThermalInfo_t

	r := C.ErmlGetDevTemp(C.uint(h.Dev_Idx), &thermal)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	thermalInfo = &DevThermalInfo{
		Cur_Dev_Temp:  float32(thermal.cur_dev_temp),
		Cur_Hbm0_Temp: float32(thermal.cur_hbm0_temp),
		Cur_Hbm1_Temp: float32(thermal.cur_hbm1_temp),
	}
	return
}

/*
 * @brief Enrigin Management Library get the device temperature.
 */
func (h Handle) GetDevTempV2() (thermalInfo *DevThermalInfoV2, err error) {
	var hwArch HwArch = Unknown
	hwArch, err = h.GetHwArch()
	if hwArch == GCU300 {
		var thermal C.ermlDevThermalInfoV2_t
		r := C.ErmlGetDevTempV2(C.uint(h.Dev_Idx), &thermal)
		err = errorString(r)
		thermalInfo = &DevThermalInfoV2{
			Cur_Asic_Temp:  float32(thermal.cur_asic_temp),
			Cur_Mem_Temp:   float32(thermal.cur_mem_temp),
			Cur_Board_Temp: float32(thermal.cur_board_temp),
		}
	} else if hwArch == GCU310 {
		var thermal C.ermlDevThermalInfoV2_t
		r := C.ErmlGetDevTempV2(C.uint(h.Dev_Idx), &thermal)
		err = errorString(r)
		thermalInfo = &DevThermalInfoV2{
			Cur_Asic_Temp:  float32(thermal.cur_asic_temp),
			Cur_Mem_Temp:   float32(thermal.cur_mem_temp),
			Cur_Board_Temp: float32(thermal.cur_board_temp),
		}
	} else {
		var thermalV1 *DevThermalInfo
		thermalV1, err = h.GetDevTemp()
		thermalInfo = &DevThermalInfoV2{
			Cur_Asic_Temp:  thermalV1.Cur_Dev_Temp,
			Cur_Mem_Temp:   thermalV1.Cur_Hbm0_Temp,
			Cur_Board_Temp: thermalV1.Cur_Dev_Temp,
		}
	}
	return
}

/*
 * @brief Enrigin Management Library get the device current power consumption.
 */
func (h Handle) GetDevPwr() (powerInfo *DevPowerInfo, err error) {
	var power C.ermlDevPowerInfo_t

	r := C.ErmlGetDevPwr(C.uint(h.Dev_Idx), &power)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	powerInfo = &DevPowerInfo{
		Pwr_Capability:      float32(power.pwr_capability),
		Cur_Pwr_Consumption: float32(power.cur_pwr_consumption),
	}
	return
}

/*
 * @brief Enrigin Management Library get the device DPM level.
 */
func (h Handle) GetDevDpmLevel() (uint, error) {
	var dpm_Level C.uint
	r := C.ErmlGetDevDpmLevel(C.uint(h.Dev_Idx), &dpm_Level)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(dpm_Level), errorString(r)
}

/*
 * @brief Enrigin Management Library get the device mem info.
 */
func (h Handle) GetDevMem() (memInfo *DevMemInfo, err error) {
	var mem C.ermlDevMemInfo_t

	r := C.ErmlGetDevMem(C.uint(h.Dev_Idx), &mem)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	memInfo = &DevMemInfo{
		Mem_Total_Size: uint(mem.mem_total_size),
		Mem_Used:       uint(mem.mem_used),
	}
	return
}

/*
 * @brief Enrigin Management Library get the device usage info.
 */
func (h Handle) GetDevDtuUsage() (float32, error) {
	var usage C.float
	r := C.ErmlGetDevDtuUsage(C.uint(h.Dev_Idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/*
 * @brief Enrigin Management Library get the device usage info by sampling and statistics.
 */
func (h Handle) GetDevDtuUsageAsync() (float32, error) {
	var usage C.float
	r := C.ErmlGetDevDtuUsageAsync(C.uint(h.Dev_Idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/**
 * @brief Enrigin Management Library get the device cluster usage.
 *
 */
func (h Handle) GetClusterUsage(cluster_idx uint) (float32, error) {
	var usage C.float
	r := C.ErmlGetDevClusterUsage(C.uint(h.Dev_Idx), C.uint(cluster_idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/**
 * @brief Enrigin Management Library get the cluster hbm memory.
 *
 */
func (h Handle) GetDevClusterHbmMem(cluster_idx uint) (memInfo *ClusterHbmMemInfo, err error) {
	var mem C.ermlClusterHbmMemInfo_t

	r := C.ErmlGetDevClusterHbmMem(C.uint(h.Dev_Idx), C.uint(cluster_idx), &mem)
	err = errorString(r)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, err
	}

	memInfo = &ClusterHbmMemInfo{
		Mem_Total_Size: uint(mem.mem_total_size),
		Mem_Used:       uint(mem.mem_used),
	}
	return
}

/**
 * @brief Enrigin Management Library get the device health stauts.
 *
 */
func (h Handle) GetDevHealth() (bool, error) {
	var health C.bool
	r := C.ErmlGetDevHealth(C.uint(h.Dev_Idx), &health)
	err := errorString(r)
	return bool(health), err
}

/**
 * @brief Enrigin Management Library get if dtu is health.
 *
 */
 func (h Handle) GetDevIsHealth() (bool, error) {
	var health C.bool
	r := C.ErmlGetDevIsHealth(C.uint(h.Dev_Idx), &health)
	err := errorString(r)
	return bool(health), err
}
/*
 * @brief Enrigin Management Library get the device clock info.
 */
func (h Handle) GetDevClk() (clkInfo *DevClkInfo, err error) {
	var clk C.ermlDevClkInfo_t

	r := C.ErmlGetDevClk(C.uint(h.Dev_Idx), &clk)
	err = errorString(r)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		clkInfo = &DevClkInfo{
			Cur_Hbm_Clock: uint(0),
			Cur_Dtu_Clock: uint(clk.cur_dtu_clock),
		}
		return
	}

	clkInfo = &DevClkInfo{
		Cur_Hbm_Clock: uint(clk.cur_hbm_clock),
		Cur_Dtu_Clock: uint(clk.cur_dtu_clock),
	}
	return
}

/*
 * @brief Enrigin Management Library get device info.
 */
func (h Handle) GetDevInfo() (devInfo *DeviceInfo, err error) {
	var dev C.ermlDeviceInfo_t

	r := C.ErmlGetDevInfo(C.uint(h.Dev_Idx), &dev)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	devInfo = &DeviceInfo{
		Name:      C.GoString(&dev.name[0]),
		Vendor_Id: uint(dev.vendor_id),
		Device_Id: uint(dev.device_id),
		Domain_Id: uint(dev.domain_id),
		Bus_Id:    uint(dev.bus_id),
		Dev_Id:    uint(dev.dev_id),
		Func_Id:   uint(dev.func_id),
	}
	return
}

/*
 * @brief Enrigin Management Library get firmware version info.
 */
func (h Handle) GetFwVersion() (string, error) {
	var ver [szName]C.char

	r := C.ErmlGetFwVersion(C.uint(h.Dev_Idx), &ver[0])
	return C.GoString(&ver[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get device UUID info.
 */
func (h Handle) getDevUuid_v1() (string /* uuid */, error) {
	var uuid [szUUID]C.char

	r := C.ErmlGetDevUuid(C.uint(h.Dev_Idx), &uuid[0])
	return C.GoString(&uuid[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get the device pg count.
 */
func (h Handle) GetDevPGCount() (uint, error) {
	var pg_cnt C.uint
	r := C.ErmlGetPGCount(C.uint(h.Dev_Idx), &pg_cnt)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(pg_cnt), errorString(r)
}

/**
 * @brief Enrigin Management Library get the pg usage.
 *
 */
func (h Handle) GetPGUsage(pg_idx uint) (float32, error) {
	var usage C.float
	r := C.ErmlGetDevPGUsage(C.uint(h.Dev_Idx), C.uint(pg_idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/**
 * @brief Enrigin Management Library get the pg usage by sampling and statistics.
 *
 */
func (h Handle) GetPGUsageAsync(pg_idx uint) (float32, error) {
	var usage C.float
	r := C.ErmlGetDevPGUsageAsync(C.uint(h.Dev_Idx), C.uint(pg_idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/**
 * @brief Enrigin Management Library get event message.
 *
 */
func GetEvent(timeout_ms int) (event_info *EventInfo, err error) {
	var event C.ermlEvent_t
	r := C.ErmlGetEvent(C.int(timeout_ms), &event)
	err = errorString(r)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, err
	} else if r == C.ERML_ERROR_TIMEOUT {
		return nil, err
	}

	event_info = &EventInfo{
		Id:   uint(C.uint(event.event_id)),
		Type: uint(C.uint(event.event_type)),
		Msg:  C.GoString(&event.event_msg[0]),
	}
	return
}

/**
 * @brief Enrigin Management Library start listen device upstream message.
 *
 */
func (h Handle) StartListenEvent() (err error) {
	var hwArch HwArch = Unknown
	hwArch, err = h.GetHwArch()
	if hwArch == GCU300 || hwArch == GCU310 {
		r := C.ErmlStartListenEvent(C.uint(h.Dev_Idx))
		err = errorString(r)
	}
	return
}

func (h Handle) getDevUuid_v2() (string, error) {
	filePath, _ := h.GetBusId()
	filePath += "/ssm/chipid"
	file, err := os.Open(filePath)
	if err != nil {
		fmt.Println("can't open file:", err.Error())
		return strconv.Itoa(0), err
	}
	defer file.Close()
	reader := bufio.NewReader(file)

	line, _, err := reader.ReadLine()
	return string(line), err
}

func (h Handle) GetDevUuidFromErml() (string, error) {
	return h.getDevUuid_v1()
}

func (h Handle) GetDevUuidFromDriver() (string, error) {
	return h.getDevUuid_v2()
}

func (h Handle) GetDevUuid() (string, error) {
	return h.getDevUuid_v1()
}

/*
 * @brief Enrigin Management Library select one target device by index.
 */
func (h Handle) SelDevByIndex() error {
	return errorString(C.ErmlSelDevByIndex(C.uint(h.Dev_Idx)))
}

/*
 * @brief Enrigin Management Library get current device pcie link speed.
 */
func (h Handle) GetPcieLinkSpeed() (uint, error) {
	var linkSpeed C.ermlPcieSpeed_t

	r := C.ErmlGetPcieLinkSpeed(C.uint(h.Dev_Idx), &linkSpeed)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(linkSpeed), errorString(r)
}

/*
 * @brief Enrigin Management Library get device hardware arch.
 */
func (h Handle) GetHwArch() (HwArch, error) {
	var hwArch C.ermlHwArch_t

	r := C.ErmlGetHwArch(C.uint(h.Dev_Idx), &hwArch)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}
	return HwArch(hwArch), errorString(r)
}

/*
 * @brief Enrigin Management Library get device hardware arch name.
 */
 func (h Handle) GetHwArchName() (string, error) {
	var archName [szName]C.char

	r := C.ErmlGetHwArchName(C.uint(h.Dev_Idx), &archName[0])
	return C.GoString(&archName[0]), errorString(r)
 }

/*
 * @brief Enrigin Management Library get current device pcie link width.
 */
func (h Handle) GetPcieLinkWidth() (uint, error) {
	var linkWidth C.ermlPcieWidth_t

	r := C.ErmlGetPcieLinkWidth(C.uint(h.Dev_Idx), &linkWidth)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(linkWidth), errorString(r)
}

/*
 * @brief Enrigin Management Library get current device pcie link info.
 */
func (h Handle) GetPcieLinkInfo() (linkInfo *LinkInfo, err error) {
	var pcie_Linkinfo C.ermlPcieLinkInfo_t

	r := C.ErmlGetPcieLinkInfo(C.uint(h.Dev_Idx), &pcie_Linkinfo)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	linkInfo = &LinkInfo{
		Link_Speed:     uint(C.uint(pcie_Linkinfo.link_speed)),
		Max_Link_Speed: uint(C.uint(pcie_Linkinfo.max_link_speed)),
		Link_Width:     uint(C.uint(pcie_Linkinfo.link_width)),
		Max_Link_Width: uint(C.uint(pcie_Linkinfo.max_link_width)),
	}
	return
}

/*
 * @brief Enrigin Management Library get pcie throughput info.
 */
func (h Handle) GetPcieThroughput() (throughputInfo *ThroughputInfo, err error) {
	var throughPut C.ermlPcieThroughputInfo_t

	r := C.ErmlGetPcieThroughput(C.uint(h.Dev_Idx), &throughPut)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	throughputInfo = &ThroughputInfo{
		Tx_Throughput: float32(throughPut.tx_throughput),
		Rx_Throughput: float32(throughPut.rx_throughput),
		Tx_Nak:        uint64(throughPut.tx_nak),
		Rx_Nak:        uint64(throughPut.rx_nak),
	}
	return
}

/*
 * @brief Enrigin Management Library get dtu rma status.
 */
func (h Handle) GetDevRmaStatus() (rmaStatus *DevRmaStatus, err error) {
	var rma C.ermlRmaStatus_t
	r := C.ErmlGetDevRmaStatus(C.uint(h.Dev_Idx), &rma)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	rmaStatus = &DevRmaStatus{
		SupportRma: bool(rma.is_dtu_support_rma),
		Flags:      bool(rma.flags),
	}
	return
}

/*
 * @brief Enrigin Management Library get dtu rma details.
 */
func (h Handle) GetDevRmaDetails() (rmaDetails *DevRmaDetails, err error) {
	var rma C.ermlRmaDetails_t
	r := C.ErmlGetDevRmaDetails(C.uint(h.Dev_Idx), &rma)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	rmaDetails = &DevRmaDetails{
		SupportRma: bool(rma.is_dtu_support_rma),
		Flags:      bool(rma.flags),
		Dbe:        uint(rma.dbe_count),
	}
	return
}

/*
 * @brief Enrigin Management Library get dram ECC status.
 */
func (h Handle) GetDevEccStatus() (eccStatus *DevEccStatus, err error) {
	var ecc C.ermlEccStatus_t
	r := C.ErmlGetDevEccStatus(C.uint(h.Dev_Idx), &ecc)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	eccStatus = &DevEccStatus{
		Enabled: bool(ecc.enabled),
		Pending: bool(ecc.pending),
		Pdblack: bool(ecc.pdblack),
		Ecnt_sb: uint(ecc.ecnt_sb),
		Ecnt_db: uint(ecc.ecnt_db),
	}
	return
}

/*
 * @brief Enrigin Management Library get total ccix port numbers.
 */
func (h Handle) GetEslPortNum() (uint, error) {
	var num C.uint
	r := C.ErmlGetEslPortNum(C.uint(h.Dev_Idx), &num)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(num), errorString(r)
}

/*
 * @brief Enrigin Management Library get ccix port info.
 */
func (h Handle) GetEslPortInfo(port_id uint) (portInfo *EslPortInfo, err error) {
	var ccixPort C.ermlEslPortInfo_t

	r := C.ErmlGetEslPortInfo(C.uint(h.Dev_Idx), C.uint(port_id), &ccixPort)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}
	err = errorString(r)
	portInfo = &EslPortInfo{
		Connected: uint(ccixPort.connected),
		Uuid:      C.GoString(&ccixPort.uuid[0]),
		Vendor_Id: uint(ccixPort.vendor_id),
		Device_Id: uint(ccixPort.device_id),
		Domain_Id: uint(ccixPort.domain_id),
		Bus_Id:    uint(ccixPort.bus_id),
		Dev_Id:    uint(ccixPort.dev_id),
		Func_Id:   uint(ccixPort.func_id),
		Port_Id:   uint(ccixPort.port_id),
		Port_Type: uint(C.uint(ccixPort.port_type)),

		Remote_Card_Id:   uint(ccixPort.remote_card_id),
		Remote_Uuid:      C.GoString(&ccixPort.remote_uuid[0]),
		Remote_Vendor_Id: uint(ccixPort.remote_vendor_id),
		Remote_Device_Id: uint(ccixPort.remote_device_id),
		Remote_Domain_Id: uint(ccixPort.remote_domain_id),
		Remote_Bus_Id:    uint(ccixPort.remote_bus_id),
		Remote_Dev_Id:    uint(ccixPort.remote_dev_id),
		Remote_Func_Id:   uint(ccixPort.remote_func_id),
		Remote_Port_Id:   uint(ccixPort.remote_port_id),
		Remote_Port_Type: uint(C.uint(ccixPort.remote_port_type)),
	}
	return
}

/*
 * @brief Enrigin Management Library get ccix link info.
 */
func (h Handle) GetEslLinkInfo(port_id uint) (linkInfo *LinkInfo, err error) {
	var ccix_Linkinfo C.ermlEslLinkInfo_t

	r := C.ErmlGetEslLinkInfo(C.uint(h.Dev_Idx), C.uint(port_id), &ccix_Linkinfo)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}
	err = errorString(r)

	linkInfo = &LinkInfo{
		Link_Speed:     uint(C.uint(ccix_Linkinfo.link_speed)),
		Max_Link_Speed: uint(C.uint(ccix_Linkinfo.max_link_speed)),
		Link_Width:     uint(C.uint(ccix_Linkinfo.link_width)),
		Max_Link_Width: uint(C.uint(ccix_Linkinfo.max_link_width)),
	}

	return
}

/*
 * @brief Enrigin Management Library get ccix dtuid info.
 */
func (h Handle) GetEslDtuId() (uint, error) {
	var id C.uint
	r := C.ErmlGetEslDtuId(C.uint(h.Dev_Idx), &id)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(id), errorString(r)
}

/*
 * @brief Enrigin Management Library get ccix throughput info.
 */
func (h Handle) GetEslThroughput(port_id uint) (throughputInfo *ThroughputInfo, err error) {
	var ccixThroughPut C.ermlEslThroughputInfo_t

	r := C.ErmlGetEslThroughput(C.uint(h.Dev_Idx), C.uint(port_id), &ccixThroughPut)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	throughputInfo = &ThroughputInfo{
		Tx_Throughput: float32(ccixThroughPut.tx_throughput),
		Rx_Throughput: float32(ccixThroughPut.rx_throughput),
		Tx_Nak:        uint64(ccixThroughPut.tx_nak),
		Rx_Nak:        uint64(ccixThroughPut.rx_nak),
	}
	return
}

func (h Handle) GetSsmFwHeartBeat() (count uint, err error) {
	filePath, _ := h.GetBusId()
	filePath += "/ssm/count"
	if _, err := os.Lstat(filePath); err == nil {
		file, err := os.Open(filePath)
		if err != nil {
			fmt.Println("can't open file:", err.Error())
			return 0, err
		}
		defer file.Close()

		reader := bufio.NewReader(file)
		line, _, err := reader.ReadLine()
		ssm_count, _ := strconv.Atoi(string(line))
		return uint(ssm_count), err
	} else {
		return 0, err
	}
}

func (h Handle) GetDevMajorMain() (major uint, main uint, err error) {
	bus_id_path, _ := h.GetBusId()
	logic_id, err := h.GetLogicId()
	if err != nil {
		return 0, 0, err
	}
	filePath := bus_id_path + "/enrigin/gcu" + strconv.Itoa(int(logic_id)) + "/dev"
	if _, err := os.Lstat(filePath); err == nil {
		file, err := os.Open(filePath)
		if err != nil {
			fmt.Println("can't open file:", err.Error())
			return 0, 0, err
		}
		defer file.Close()
		reader := bufio.NewReader(file)

		line, _, err := reader.ReadLine()
		slice := strings.Split(string(line), ":")
		major, _ := strconv.Atoi(slice[0])
		main, _ := strconv.Atoi(slice[1])
		return uint(major), uint(main), err
	} else {
		return 0, 0, err
	}
}

func (h Handle) GetDevState() (state string, err error) {
	filePath, _ := h.GetBusId()
	filePath += "/device_state"
	if _, err := os.Lstat(filePath); err == nil {
		file, err := os.Open(filePath)
		if err != nil {
			fmt.Println("can't open file:", err.Error())
			return "", err
		}
		defer file.Close()
		reader := bufio.NewReader(file)

		line, _, err := reader.ReadLine()
		return string(line), err
	} else {
		return "", err
	}
}

func (h Handle) GetDevInSleepMode() (sleep uint, err error) {
	filePath, _ := h.GetBusId()
	filePath += "/ssm/status"
	if _, err := os.Lstat(filePath); err == nil {
		file, err := os.Open(filePath)
		if err != nil {
			fmt.Println("can't open file:", err.Error())
			return 0, err
		}
		defer file.Close()
		r := bufio.NewReader(file)

		line, _, err := r.ReadLine()
		sleep, _ := strconv.Atoi(string(line))
		return uint(sleep), err
	} else {
		return 0, err
	}
}

func GetDeviceHandleByIndex(dev_idx uint) (Handle, error) {
	var h Handle
	h = Handle{
		Dev_Idx: dev_idx,
	}
	return h, nil
}

func (h Handle) GetDevSKU() (string, error) {
	var devSKU [szName]C.char

	r := C.ErmlGetDevSKU(C.uint(h.Dev_Idx), &devSKU[0])
	return C.GoString(&devSKU[0]), errorString(r)
}

func (h Handle) GetDevSn() (string, error) {
	var devSn [szName]C.char

	r := C.ErmlGetDevSn(C.uint(h.Dev_Idx), &devSn[0])
	return C.GoString(&devSn[0]), errorString(r)
}

func (h Handle) GetDevPn() (string, error) {
	var devPn [szName]C.char

	r := C.ErmlGetDevPn(C.uint(h.Dev_Idx), &devPn[0])
	return C.GoString(&devPn[0]), errorString(r)
}

/*
 * @brief Enrigin Management Library get the total number of virtual devices per device.
 */
func (h Handle) GetVdevCount() (uint, error) {
	var vdev_cnt C.uint
	r := C.ErmlGetVdevCount(C.uint(h.Dev_Idx), &vdev_cnt)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return uint(vdev_cnt), errorString(r)
}

/*
 * @brief Enrigin Management Library get virtual devices index in os.
 */
func (h Handle) GetVdevList() (vdevList []uint, err error) {
	var count C.uint32_t
	var vDevIds [32]C.uint32_t
	r := C.ErmlGetVdevList(C.uint(h.Dev_Idx), &vDevIds[0], &count)
	err = errorString(r)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, err
	}
	for i := uint(0); i < uint(count); i++ {
		vdevList = append(vdevList, uint(vDevIds[i]))
	}

	return
}

/*
 * @brief Enrigin Management Library get the virtual device mem info.
 */
func (h Handle) GetVdevDtuMem(vdev_idx uint) (memInfo *DevMemInfo, err error) {
	var mem C.ermlDevMemInfo_t

	r := C.ErmlGetVdevMem(C.uint(h.Dev_Idx), C.uint(vdev_idx), &mem)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return nil, errorString(r)
	}

	err = errorString(r)

	memInfo = &DevMemInfo{
		Mem_Total_Size: uint(mem.mem_total_size),
		Mem_Used:       uint(mem.mem_used),
	}
	return
}

/*
 * @brief Enrigin Management Library get the virtual device usage.
 */
func (h Handle) GetVdevDtuUsage(vdev_idx uint) (float32, error) {
	var usage C.float
	r := C.ErmlGetVdevDtuUsage(C.uint(h.Dev_Idx), C.uint(vdev_idx), &usage)
	if r == C.ERML_ERROR_NOT_SUPPORTED {
		return 0, errorString(r)
	}

	return float32(usage), errorString(r)
}

/*
 * @brief Enrigin Management Library get process info on device.
 */
func (h Handle) GetProcessInfo() (pInfos []ProcessInfo, err error) {
	var count C.uint32_t
	var processInfos [64]C.ermlProcessInfo_t
	r := C.ErmlGetProcessInfo(C.uint(h.Dev_Idx), &count, &processInfos[0])
	err = errorString(r)
	for i := uint(0); i < uint(count); i++ {
		pInfos = append(pInfos, ProcessInfo{
			Pid:         uint(processInfos[i].pid),
			DevMemUsage: uint64(processInfos[i].dev_mem_usage),
			SysMemUsage: uint64(processInfos[i].sys_mem_usage),
		})
	}

	return
}
