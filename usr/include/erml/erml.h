/////////////////////////////////////////////////////////////////////////////
//
//  @file erml.h
//
//  @brief API interface of Enrigin Managerment Library
//
//  Enrigin Tech, All Rights Reserved. 2024 Copyright (C)
//
/////////////////////////////////////////////////////////////////////////////

#ifndef LIB_ERML_INCLUDE_ERML_H_
#define LIB_ERML_INCLUDE_ERML_H_

#include <stdint.h>

#include "erml/erml_export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ERML_SUCCESS                     = 0,  // Success
    ERML_ERROR_UNINITIALIZED         = 1,  // Error since uninitialized.
    ERML_ERROR_INVALID_ARGUMENT      = 2,
    ERML_ERROR_NOT_SUPPORTED         = 3,
    ERML_ERROR_LIBRARY_NOT_FOUND     = 4,
    ERML_ERROR_INVALID_ERROR_CODE    = 5,
    ERML_ERROR_DRIVER_NOT_LOADED     = 6,
    ERML_ERROR_ESL_PORT_NUMBER_ERR   = 7,
    ERML_ERROR_INVALID_INPUT         = 8,
    ERML_ERROR_FUNCTION_NOT_FOUND    = 9,
    ERML_ERROR_OPEN_DRIVER_VERSION   = 10,
    ERML_ERROR_DRIVER_NOT_COMPATIBLE = 11,
    ERML_ERROR_NO_DEVICE             = 12,
    ERML_ERROR_TIMEOUT               = 253,
    ERML_ERROR_FAIL                  = 254,
    ERML_ERROR_MAX                   = 255,
} ermlReturn_t;

typedef enum {
    ERML_LINK_SPEED_GEN1 = 1,
    ERML_LINK_SPEED_GEN2 = 2,
    ERML_LINK_SPEED_GEN3 = 3,
    ERML_LINK_SPEED_GEN4 = 4,
    ERML_LINK_SPEED_GEN5 = 5,
} ermlPcieSpeed_t;

typedef enum {
    GCU200 = 0,
    GCU210 = 1,
    GCU300 = 2,
    GCU310 = 3,
} ermlHwArch_t;

typedef enum {
    UNKNOW  = 0,
    GCU_ARCH_200  = 20, /* pavo */
    GCU_ARCH_210  = 21, /* dorado */
    GCU_ARCH_300  = 30, /* scorpio */
    GCU_ARCH_310  = 31, /* dorstar */
    GCU_ARCH_400  = 40, /* libra */
} ermlHwArchV2_t;

typedef enum {
    PERFORMANCE_UNKNOWN_MODE  = 0,
    PERFORMANCE_USER_MODE     = 1,
    PERFORMANCE_BOOST_MODE    = 2,
    PERFORMANCE_ADAPTIVE_MODE = 3,
    PERFORMANCE_ENERGY_MODE   = 4,
    PERFORMANCE_MODE_MAX,
} ermlPerfMode_t;

typedef enum {
    CABLE_NOT_QUALIFIED = 0,
    CABLE_QUALIFIED     = 1,
} ermlCableQualifySts_t;

typedef enum {
    ERML_LINK_WIDTH_X1  = 1,
    ERML_LINK_WIDTH_X2  = 2,
    ERML_LINK_WIDTH_X4  = 4,
    ERML_LINK_WIDTH_X8  = 8,
    ERML_LINK_WIDTH_X16 = 16,
} ermlPcieWidth_t;

typedef enum {
    ERML_ESL_LINK_SPEED_GEN1      = 1,
    ERML_ESL_LINK_SPEED_GEN2      = 2,
    ERML_ESL_LINK_SPEED_GEN3      = 3,
    ERML_ESL_LINK_SPEED_GEN4      = 4,
    ERML_ESL_LINK_SPEED_GEN5      = 5,
    ERML_ESL_LINK_SPEED_ESM_2P5GT = 0x10,
    ERML_ESL_LINK_SPEED_ESM_5GT,
    ERML_ESL_LINK_SPEED_ESM_8GT,
    ERML_ESL_LINK_SPEED_ESM_16GT,
    ERML_ESL_LINK_SPEED_ESM_20GT,
    ERML_ESL_LINK_SPEED_ESM_25GT
} ermlEslSpeed_t;

typedef enum {
    ERML_ESL_LINK_WIDTH_X1  = 1,
    ERML_ESL_LINK_WIDTH_X2  = 2,
    ERML_ESL_LINK_WIDTH_X4  = 4,
    ERML_ESL_LINK_WIDTH_X8  = 8,
    ERML_ESL_LINK_WIDTH_X16 = 16,
} ermlEslWidth_t;

typedef enum {
    ERML_ESL_PORT_RC = 0,
    ERML_ESL_PORT_EP = 1,
} ermlEslPortType_t;

typedef enum {
    ERML_HBM_SCAN_INVALID = 0,
    ERML_HBM_SCAN_START   = 1,
} ermlHbmScanType_t;

typedef enum {
    ERML_EVENT_UNKNOWN          = 0,
    ERML_EVENT_DTU_SUSPEND      = 3,
    ERML_EVENT_DTU_RESET_START  = 10,
    ERML_EVENT_DTU_RESET_FINISH = 11,
} ermlEventType_t;

typedef enum {
    ERML_GCU_VIRT_MODE_HOST,
    ERML_GCU_VIRT_MODE_VGCU,
    ERML_GCU_VIRT_MODE_MDEV,
    ERML_GCU_VIRT_MODE_SRIOV_PF,
    ERML_GCU_VIRT_MODE_SRIOV_VF,
} ermlGcuVirtMode_t;

#define MAX_CHAR_BUFF_LEN 128

/**
 * @brief Enrigin data structure for device info.
 *
 */
typedef struct {
    char     name[MAX_CHAR_BUFF_LEN];
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t domain_id;
    uint32_t bus_id;
    uint32_t dev_id;
    uint32_t func_id;
    uint32_t logic_id;
} ermlDeviceInfo_t;

/**
* @brief Enrigin data structure for device power info.
*
*/
typedef struct {
    float pwr_capability;
    float cur_pwr_consumption;
} ermlDevPowerInfo_t;

/**
* @brief Enrigin data structure for device memory info.
*
*/
typedef struct {
    uint64_t mem_total_size;
    uint32_t mem_used;
} ermlDevMemInfo_t;

typedef struct {
    uint32_t        event_id;
    ermlEventType_t event_type;
    char            event_msg[MAX_CHAR_BUFF_LEN];
} ermlEvent_t;

/**
* @brief Enrigin data structure for cluster memory info.
*
*/
typedef struct {
    uint64_t mem_total_size;
    uint32_t mem_used;
} ermlClusterHbmMemInfo_t;

/**
* @brief Enrigin data structure for device clock info.
*
*/
typedef struct {
    uint32_t cur_hbm_clock;
    uint32_t cur_dtu_clock;
} ermlDevClkInfo_t;

/**
* @brief Enrigin data structure for device thermal info.
*
*/
typedef struct {
    float cur_dev_temp;
    float cur_hbm0_temp;
    float cur_hbm1_temp;
} ermlDevThermalInfo_t;

typedef struct {
    float cur_asic_temp;
    float cur_mem_temp;
    float cur_board_temp;
} ermlDevThermalInfoV2_t;

/**
* @brief Enrigin data structure for device volt info.
*
*/
typedef struct {
    float vdd_dtu;
    float vdd_soc;
    float vdd_hbmqc;
    float vdd_1v8;
    float vdd_vddp;
} ermlDevVoltInfo_t;

/**
 * @brief Enrigin data structure for Pcie link info.
 *
 */
typedef struct {
    ermlPcieSpeed_t link_speed;
    ermlPcieSpeed_t max_link_speed;
    ermlPcieWidth_t link_width;
    ermlPcieWidth_t max_link_width;
} ermlPcieLinkInfo_t;

/**
* @brief Enrigin data structure for Pcie Throughput info.
*
*/
typedef struct {
    float    tx_throughput;
    float    rx_throughput;
    uint64_t tx_nak;
    uint64_t rx_nak;
} ermlPcieThroughputInfo_t;

/**
* @brief Enrigin data structure for esl link info.
*
*/
typedef struct {
    ermlEslSpeed_t link_speed;
    ermlEslSpeed_t max_link_speed;
    ermlEslWidth_t link_width;
    ermlEslWidth_t max_link_width;
} ermlEslLinkInfo_t;

/**
*@brief Enrigin data structure for Esl Throughput info.
*
*/
typedef struct {
    float    tx_throughput;
    float    rx_throughput;
    uint64_t tx_nak;
    uint64_t rx_nak;
} ermlEslThroughputInfo_t;

/**
 * @brief Enrigin data structure for esl port info.
 *
 */
typedef struct {
    uint32_t          connected;
    char              uuid[16];
    uint32_t          vendor_id;
    uint32_t          device_id;
    uint32_t          domain_id;
    uint32_t          bus_id;
    uint32_t          dev_id;
    uint32_t          func_id;
    uint32_t          port_id;
    ermlEslPortType_t port_type;

    uint32_t          remote_card_id;
    char              remote_uuid[16];
    uint32_t          remote_vendor_id;
    uint32_t          remote_device_id;
    uint32_t          remote_domain_id;
    uint32_t          remote_bus_id;
    uint32_t          remote_dev_id;
    uint32_t          remote_func_id;
    uint32_t          remote_port_id;
    ermlEslPortType_t remote_port_type;
} ermlEslPortInfo_t;

/**
 * @brief Enrigin Device Handle or The idenfifier of the target device.
 *
 */
typedef struct { void *handle; } ermlDevice_t;

/**
*@brief Enrigin data structure for Esl Throughput info.
*
*/
typedef struct {
    bool     enabled;  // true: enabled               false: disabled
    bool     pending;  // true: pending               false: no pending
    bool     pdblack;  // true: pending blacklist yes false: no
    uint32_t ecnt_sb;  // single bit error count
    uint32_t ecnt_db;  // double bit error count
} ermlEccStatus_t;

/**
*@brief Enrigin data structure for Rma status.
*
*/
typedef struct {
    bool is_dtu_support_rma;
    bool flags;  // true: hint, false: not hint
} ermlRmaStatus_t;

/**
*@brief Enrigin data structure for Rma details.
*
*/
typedef struct {
    bool     is_dtu_support_rma;
    bool     flags;  // true: hint, false: not hint
    uint32_t dbe_count;
} ermlRmaDetails_t;

/**
*@brief Enrigin data structure for Decoder info.
*
*/
typedef struct {
    uint32_t decoder_inst_num;    // decoder instance number
    float    decoder_resolution;  // decoder resolution
    float    decoder_fps;         // decoder fps
} ermlDecoderCap_t;

/**
*@brief Enrigin data structure for HBM usage info.
*
*/
typedef struct {
    uint64_t hbm_total;
    uint64_t hbm_free;
    uint64_t hbm_used;
} ermlHbmUsage_t;

/**
*@brief Enrigin data structure for SIP usage info.
*
*/
typedef struct {
    uint32_t sip_total;
    uint32_t sip_init;
    uint32_t sip_idle;
    uint32_t sip_busy;
    uint32_t sip_masked;
    uint32_t sip_hwerr;
} ermlSipUsage_t;

/**
*@brief Enrigin data structure for CQM usage info.
*
*/
typedef struct {
    uint32_t cqm_total;
    uint32_t cqm_init;
    uint32_t cqm_idle;
    uint32_t cqm_busy;
    uint32_t cqm_masked;
    uint32_t cqm_hwerr;
} ermlCqmUsage_t;

/**
*@brief Enrigin data structure for DTU usage info.
*
*/
typedef struct {
    ermlHbmUsage_t hbm;
    ermlSipUsage_t sip;
    ermlCqmUsage_t cqm;
} ermlDtuUsage_t;

typedef struct {
    uint32_t pid;           /* process pid         */
    uint64_t dev_mem_usage; /* device memory usage */
    uint64_t sys_mem_usage; /* system memory usage */
} ermlProcessInfo_t;

typedef enum {
      OP_SPACE_GCU,
      OP_SPACE_PF,
      OP_SPACE_CTL,
} ermlOperateSpace_t;

/**
 * @brief Enrigin Management Library initialization
 *
 * @return ermlReturn_t
 */
ERML_EXPORT ermlReturn_t ErmlInit(bool no_driver);

/**
 * @brief Enrigin Management Library Shutdown.
 *
 */
ERML_EXPORT void ErmlShutdown();

/**
 * @brief Enrigin Management Library map error code to string.
 *
 */
ERML_EXPORT ermlReturn_t ErmlErrorString(ermlReturn_t result, char *p_error_str);

/**
 * @brief Enrigin Management Library get driver version info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDriverVer(char *p_driver_ver);

/**
 * @brief Enrigin Management Library get erml lib version info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetLibVer(char *p_self_ver);

/**
 * @brief Enrigin Management Library get enrigin driver status and config access path.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDriverAccessPoint(char *p_enrigin_driver_ap);

/**
 * @brief Get the total number of supported devices
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevCount(uint32_t *dev_count);

/**
 * @brief Get the total number of enabled virtual devices of a physical device
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetVdevCount(uint32_t dev_idx, uint32_t *vdev_count);

/**
 * @brief Get the virtual devices index in operate system
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetVdevList(uint32_t dev_idx, uint32_t *vdev_ids, uint32_t *count);

/**
 * @brief Get the total number of maximum supported virtual devices of a physical device
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetMaxVdevCount(uint32_t dev_idx, uint32_t *vdev_count);

/**
 * @brief Enrigin Management Library get the device name.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevName(uint32_t dev_idx, char *p_name);

/**
 * @brief Enrigin Management Library get the device temperature.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevTemp(uint32_t dev_idx, ermlDevThermalInfo_t *p_temp);

/**
 * @brief Enrigin Management Library get the device temperature.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevTempV2(uint32_t dev_idx, ermlDevThermalInfoV2_t *p_temp);

/**
 * @brief Enrigin Management Library get the device voltage.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevVolt(uint32_t dev_idx, ermlDevVoltInfo_t *p_volt);

/**
 * @brief Enrigin Management Library get the device current power consumption.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevPwr(uint32_t dev_idx, ermlDevPowerInfo_t *p_pwr);

/**
 * @brief Enrigin Management Library get the device DPM level.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevDpmLevel(uint32_t dev_idx, uint32_t *p_dpm);

/**
 * @brief Enrigin Management Library get the device mem info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevMem(uint32_t dev_idx, ermlDevMemInfo_t *p_mem);

/**
 * @brief Enrigin Management Library get the device usage.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevDtuUsage(uint32_t dev_idx, float *p_data);

/**
 * @brief Enrigin Management Library get the virtual device mem info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetVdevMem(uint32_t dev_idx, uint32_t vdev_idx, ermlDevMemInfo_t *p_mem);

/**
 * @brief Enrigin Management Library get the virtual device usage.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetVdevDtuUsage(uint32_t dev_idx, uint32_t vdev_idx, float *p_data);

/**
 * @brief Enrigin Management Library get the device usage from background sample thread.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevDtuUsageAsync(uint32_t dev_idx, float *p_data);

/**
 * @brief Enrigin Management Library get if dtu is in low power mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevIsLowPowerMode(uint32_t dev_idx, bool *is_low_power_mode);

/**
 * @brief Enrigin Management Library get if dtu is health.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevIsHealth(uint32_t dev_idx, bool *is_health);

/**
 * @brief Enrigin Management Library switch dtu low power mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSetDevSupportLowPower(uint32_t dev_idx, bool enable_low_power_support);

/**
 * @brief Enrigin Management Library get if dtu support low power mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevSupportLowPower(uint32_t dev_idx, bool *is_support_low_power);

/**
 * @brief Enrigin Management Library switch dtu power stock mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSetDevSupportPowerStock(uint32_t dev_idx, bool enable_power_stock_support);

/**
 * @brief Enrigin Management Library get if dtu support power stock mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevSupportPowerStock(uint32_t dev_idx, bool *is_support_power_stock);

/**
 * @brief Enrigin Management Library get the device max clock freqency.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetMaxFreq(uint32_t dev_idx, uint32_t *max_freq_mhz);

/**
 * @brief Enrigin Management Library set the device max clock freqency.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSetMaxFreq(uint32_t dev_idx, uint32_t max_freq_mhz);

/**
 * @brief Enrigin Management Library get the device clock info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevClk(uint32_t dev_idx, ermlDevClkInfo_t *p_clk);

/**
 * @brief Enrigin Management Library get the memory clock info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetMemClk(uint32_t dev_idx, uint32_t *p_clk_mhz);

/**
 * @brief Enrigin Management Library dump supported device list.
 *
 */
ERML_EXPORT ermlReturn_t ErmlDumpDevList(void);

/**
 * @brief Enrigin Management Library get device info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevInfo(uint32_t dev_idx, ermlDeviceInfo_t *p_info);

/**
 * @brief Enrigin Management Library get device parent info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevParentInfo(uint32_t dev_idx, ermlDeviceInfo_t *p_info);

/**
 * @brief Enrigin Management Library display device topology.
 *
 */
ERML_EXPORT ermlReturn_t ErmlDisplayDevTop(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library get firmware version info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetFwVersion(uint32_t dev_idx, char *fw_ver);

/**
 * @brief Enrigin Management Library get device UUID info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevUuid(uint32_t dev_idx, char *p_info);

/**
 * @brief Enrigin Management Library get device SN(Serial Number) info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevSn(uint32_t dev_idx, char *p_sn);

/**
 * @brief Enrigin Management Library get device PN(Part Number) info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevPn(uint32_t dev_idx, char *p_pn);

/**
 * @brief Enrigin Management Library get device Manufacturing Date.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevMfd(uint32_t dev_idx, char *p_date);

/**
 * @brief Enrigin Management Library get device SKU info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevSKU(uint32_t dev_idx, char *p_info);

/**
 * @brief Enrigin Management Library get device hw arch info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetHwArch(uint32_t dev_idx, ermlHwArch_t *p_arch);

/**
 * @brief Enrigin Management Library get device hw arch name.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetHwArchName(uint32_t dev_idx, char *arch_name);

/**
 * @brief Enrigin Management Library get power cable qualify status.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetCableQualify(
    uint32_t dev_idx, ermlCableQualifySts_t *p_sts);

/**
 * @brief Enrigin Management Library get device performance mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPerfMode(
    uint32_t dev_idx, ermlPerfMode_t *p_mode, uint32_t *kfc_lvl);

/**
 * @brief Enrigin Management Library set device performance mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSetPerfMode(
    uint32_t dev_idx, ermlPerfMode_t mode, uint32_t kfc_lvl);

// /**
//  * @brief Enrigin Management Library get device PCIe slot number.
//  *
//  */
// ERML_EXPORT ermlReturn_t ErmlGetDevSlotNum(uint32_t dev_idx, uint32_t *p_slot);

/**
 * @brief Enrigin Management Library get device PCIe slot or OAM name.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevSlotOamName(uint32_t dev_idx, char *p_slot_oam);

/**
 * @brief Enrigin Management Library select one target device by index.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSelDevByIndex(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library get current device pcie link speed.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPcieLinkSpeed(uint32_t dev_idx, ermlPcieSpeed_t *p_link_speed);

/**
 * @brief Enrigin Management Library get current device pcie link width.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPcieLinkWidth(uint32_t dev_idx, ermlPcieWidth_t *p_link_width);

/**
 * @brief Enrigin Management Library get current device pcie link info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPcieLinkInfo(uint32_t dev_idx, ermlPcieLinkInfo_t *p_link_info);

/**
 * @brief Enrigin Management Library get pcie throughput info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPcieThroughput(uint32_t dev_idx, ermlPcieThroughputInfo_t *p_info);

/**
 * @brief Enrigin Management Library pcie hot reset.
 *
 */
ERML_EXPORT ermlReturn_t ErmlPcieHotReset(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library advance pcie hot reset.
 *
 */
ERML_EXPORT ermlReturn_t ErmlPcieHotResetV2(uint32_t dev_idx, bool is_force);

/**
 * @brief Enrigin Management Library system pcie reset.
 *
 */
ERML_EXPORT ermlReturn_t ErmlPcieHotResetV3(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library advance pcie function level reset.
 *
 */
ERML_EXPORT ermlReturn_t ErmlPcieFLR(uint32_t dev_idx, bool is_force);

/**
 * @brief Enrigin Management Library get pcie slot ID.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPciePhysicalSlotID(uint32_t dev_idx, uint32_t *p_id);

/**
 * @brief Enrigin Management Library get pcie up top.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevTop(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library get dram ECC status.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEccStatus(uint32_t dev_idx, uint32_t *status);

/**
 * @brief Enrigin Management Library get total esl port numbers.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslPortNum(uint32_t dev_idx, uint32_t *p_data);

/**
 * @brief Enrigin Management Library get esl port info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslPortInfo(uint32_t dev_idx, uint32_t port_id, ermlEslPortInfo_t *p_info);

/**
 * @brief Enrigin Management Library get esl link info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslLinkInfo(uint32_t dev_idx, uint32_t port_id, ermlEslLinkInfo_t *p_info);

/**
 * @brief Enrigin Management Library get esl dtuid info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslDtuId(uint32_t dev_idx, uint32_t *p_data);

/**
 * @brief Enrigin Management Library get esl support info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslIsSupported(uint32_t dev_idx, bool *is_esl_supported);

/**
 * @brief Enrigin Management Library get esl throughput info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEslThroughput(
    uint32_t dev_idx, uint32_t port_id, ermlEslThroughputInfo_t *p_info);

/**
 * @brief Enrigin Management Library get the total number of special device's pgs.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetPGCount(uint32_t dev_idx, uint32_t *pg_count);

/**
 * @brief Enrigin Management Library get the device pg usage.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevPGUsage(uint32_t dev_idx, uint32_t pg_idx, float *p_data);

/**
 * @brief Enrigin Management Library get the device pg usage from background sample thread..
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevPGUsageAsync(uint32_t dev_idx, uint32_t pg_idx, float *p_data);

/**
 * @brief Enrigin Management Library get the total number of special device's clusters.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetClusterCount(uint32_t dev_idx, uint32_t *cluster_count);

/**
 * @brief Enrigin Management Library get the device cluster usage.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevClusterUsage(uint32_t dev_idx, uint32_t cluster_idx, float *p_data);

/**
 * @brief Enrigin Management Library get the cluster mem info.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevClusterHbmMem(
    uint32_t dev_idx, uint32_t cluster_idx, ermlClusterHbmMemInfo_t *p_mem);

/**
 * @brief Enrigin Management Library get the device health stauts.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevHealth(uint32_t dev_idx, bool *health);

/**
 * @brief Enrigin Management Library get the device ecc stauts.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevEccStatus(uint32_t dev_idx, ermlEccStatus_t *p_status);

/**
 * @brief Enrigin Management Library get the device rma stauts.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevRmaStatus(uint32_t dev_idx, ermlRmaStatus_t *p_status);

/**
 * @brief Enrigin Management Library get the device rma details.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevRmaDetails(uint32_t dev_idx, ermlRmaDetails_t *p_details);

/**
 * @brief Enrigin Management Library get the device logic id.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevLogicId(uint32_t dev_idx, uint32_t *p_logic_id);

/**
 * @brief Enrigin Management Library get the device decoder capability.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetDevDecoderCap(uint32_t dev_idx, ermlDecoderCap_t *p_decoder_cap);

/**
 * @brief Enrigin Management Library set the device ecc mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSetDevEccMode(uint32_t dev_idx, bool enable);

/**
 * @brief Enrigin Management Library set the device hbm scan mode.
 *
 */
ERML_EXPORT ermlReturn_t ErmlHbmScanMode(uint32_t dev_idx, ermlHbmScanType_t op_type);

/**
 * @brief Enrigin Management Library get vGCU virtualization status.
 * @deprecated This API is deprecated, please use ErmlGetGcuVirtStatus instead.
 *
 */
ERML_DEPRECATED_EXPORT ermlReturn_t ErmlGetDevIsVdtuEnabled(uint32_t dev_idx, bool *is_vdtu_enabled);

/**
 * @brief Enrigin Management Library get event message.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetEvent(int timeout_ms, ermlEvent_t *p_event);

/**
 * @brief Enrigin Management Library start listen event.
 *
 */
ERML_EXPORT ermlReturn_t ErmlStartListenEvent(uint32_t dev_idx);

/**
 * @brief Enrigin Management Library switch operate space.
 *
 */
ERML_EXPORT ermlReturn_t ErmlSwitchOperateSpace(ermlOperateSpace_t opsp);

/**
 * @brief Enrigin Management Library get process info on device.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetProcessInfo(
    uint32_t dev_idx, uint32_t *process_count, ermlProcessInfo_t *p_info);

/**
 * @brief Enrigin Management Library get device numa node.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetNumaNode(uint32_t dev_idx, int *numa_node);

/**
 * @brief Enrigin Management Library get PF name of device. Only support on VF.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetSriovPfName(uint32_t dev_idx, char *pf_name);

/**
 * @brief Enrigin Management Library get device affinity CPU list.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetAffinityCpuList(uint32_t dev_idx, char *cpu_list);

/**
 * @brief Enrigin Management Library get SR-IOV status.
 *
 */
ERML_EXPORT ermlReturn_t ErmlGetGcuVirtStatus(uint32_t dev_idx, ermlGcuVirtMode_t *virt_mode);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // LIB_ERML_INCLUDE_ERML_H_
