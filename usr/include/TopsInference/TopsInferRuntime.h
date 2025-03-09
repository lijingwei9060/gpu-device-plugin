/**
 * @file TopsInferRuntime.h
 * @author www.enrigin.com.cn
 * @brief Abstract Interface for TopsInference
 * @version 0.1
 * @date 2020-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __TOPS_INFER_RUNTIME_H__
#define __TOPS_INFER_RUNTIME_H__
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "tops/tops_ext.h"

#ifdef _MSC_VER
#define TOPS_INFERENCE_EXPORT __declspec(dllexport)
#else
#define TOPS_INFERENCE_EXPORT __attribute__((visibility("default")))
#endif

// Items that are marked as deprecated will be removed in a future release.
#if __cplusplus >= 201402L
#define TIF_DEPRECATED [[deprecated]]
#if __GNUC__ < 6
#define TIF_DEPRECATED_ENUM
#else
#define TIF_DEPRECATED_ENUM TIF_DEPRECATED
#endif
#ifdef _MSC_VER
#define TIF_DEPRECATED_API __declspec(dllexport)
#else
#define TIF_DEPRECATED_API [[deprecated]] __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define TIF_DEPRECATED
#define TIF_DEPRECATED_ENUM
#define TIF_DEPRECATED_API __declspec(dllexport)
#else
#define TIF_DEPRECATED __attribute__((deprecated))
#define TIF_DEPRECATED_ENUM
#define TIF_DEPRECATED_API __attribute__((deprecated, visibility("default")))
#endif
#endif

/**
 * @mainpage
 * This is the API reference documentation for Enrigin TopsInference.
 * Information about functions, classes and methods can be found here.
 * Use the index on the left to navigate the documentation.\n
 * For more information on how to use TopsInference, please see the developer
 * guide and samples.
 *
 */
namespace TopsInference {
class ICalibrator;
//!
//! Logical modifier.
//!

/**
 * @brief It indicates that the input parameter will not be wrote but only be
 * read by current function.
 */
#define IN
/**
 * @brief It indicates that the input parameter will be read and wrote by
 * current function.
 */
#define INOUT
/**
 * @brief It indicates that the input parameter will be wrote but not be read by
 * current function.
 */
#define OUT
namespace impl {
// Declaration of EnumMaxImpl struct to store maximum number of elements in an
// enumeration type.
template <typename T>
struct EnumMaxImpl;
}  // namespace impl

// Maximum number of elements in an enumeration type.
template <typename T>
constexpr int32_t EnumMax() noexcept {
  return impl::EnumMaxImpl<T>::kVALUE;
}

/**
 *  @defgroup TIFStatus Status code
 *  @{
 *  This section describes the TopsInference Status code.
 *
 */

/**
 * @brief Status code
 */
enum TIFStatus {
  /**
   * @brief Success
   */
  TIF_SUCCESS = 0,
  /**
   * @brief Engine run inference failed
   */
  TIF_ENGINE_RUN_FAILED,
  /**
   * @brief Model is not supported by current parser
   */
  TIF_PARSER_UNSUPPORTED_MODEL,
  /**
   * @brief Optimizer build failed
   */
  TIF_OPTIMIZER_BUILD_FAILED,
  /**
   * @brief Uninitialized error
   */
  TIF_NOT_INITIALIZED,
  /**
   * @brief Internal error
   */
  TIF_INTERNAL_ERROR,
  /**
   * @brief Allocate buffer failed
   */
  TIF_ALLOCATE_FAILED,
  /**
   * @brief Invalid parameters
   */
  TIF_INVALID_PARAM,
  /**
   * @brief These value maybe is nullptr or zero
   */
  TIF_INVALID_VALUE,
  /**
   * @brief Mismatched data type
   */
  TIF_MIS_MATCHED_DATA_TYPE,
  /**
   * @brief Mismatched shape or dimension
   */
  TIF_MIS_MATCHED_SHAPE_OR_DIM,
  /**
   * @brief Mapping error
   */
  TIF_MAPPING_ERROR,
  /**
   * @brief Un-supported datatype
   */
  TIF_UN_SUPPORTED_DATA_TYPE,
  /**
   * @brief Un-supported operator
   */
  TIF_UN_SUPPORTED_OPERATOR,
  /**
   * @brief License error
   */
  TIF_LICENSE_ERROR,
  /**
   * @brief Miss prerequisite in runtime
   */
  TIF_RUNTIME_PREREQUISITE_MISSING,
  /**
   * @brief Has occur overflow
   */
  TIF_RUNTIME_FP_OVERFLOW,
  /**
   * @brief Some value is out of limit
   */
  TIF_VALIUE_OVERFLOW,
  /**
   * @brief Has some keys or names not find in lists
   */
  TIF_NOT_FOUND,
  /**
   * @brief Tensor has already existed
   */
  TIF_ALREADY_EXISTED_TENSOR,
  /**
   * @brief Operater has already existed
   */
  TIF_ALREADY_EXISTED_OP,
  /**
   * @brief Graph quantize failed
   */
  TIF_COMPILE_QUANTIZE_FAILED,
  /**
   * @brief Run graph pass failed
   */
  TIF_COMPILE_RUN_PASS_FAILED,
  /**
   * @brief Graph Error, eg.cycle Graph
   */
  TIF_COMPILE_GRAPH_ERROR,
  /**
   * @brief Unsupported model structure
   */
  TIF_COMPILE_PARSE_FAILED,
  /**
   * @brief Convert fail
   */
  TIF_COMPILE_CONVERT_FAILED,
  /**
   * @brief Load File not exist
   */
  TIF_LOAD_NON_EXIST_FILE,
  /**
   * @brief Save File not exist
   */
  TIF_SAVE_NON_EXIST_FILE,
  /**
   * @brief Load PB file not exist
   */
  TIF_LOAD_NON_EXIST_PB_FILE,
  /**
   * @brief Executable CRC verify failed
   */
  TIF_EXECUTABLE_CRC_VERIFY_FAILED,
  /**
   * @brief Executable device verify failed
   */
  TIF_EXECUTABLE_DEVICE_VERIFY_FAILED,
  /**
   * @brief Too many clusters are applied without auto serving
   */
  TIF_MORE_CLUSTER_APPLIED,
  /**
   * @brief Terminate
   *
   */
  TIF_TERMINATED,
  /**
   * @brief Executable weight refit failed
   *
   */
  TIF_EXECUTABLE_WEIGHT_REFIT_FAILED,
  /**
   * @brief Finish
   *
   */
  TIF_FINISHED,
};

// doxygen end Status
/**
 * @}
 */

/**
 *  @defgroup Type Type information
 *  @{
 *  This section describes the TopsInference Type information.
 *
 */

/**
 *  @defgroup BuildFlag
 *  @{
 *  This section describes the TopsInference BuildFlag information.
 *
 */

/**
 * @brief The flags used in Optimizer when building
 */
enum BuildFlag {
  /**
   * @brief If the flag was set, then using the building precision inside
   * for each layer, fp32 in general
   */
  TIF_KTYPE_DEFAULT = 0b1,
  /**
   * @brief Using fp16 for all layers.
   *
   */
  TIF_KTYPE_FLOAT16 = 0b10,
  /**
   * @brief Using f16 for each layer except that some layers which are
   * sensitive to precision will use f32
   *
   */
  TIF_KTYPE_MIX_FP16 = 0b100,
  /**
   * @brief Using int8 for each layer except that some layers which are
   * sensitive to precision will use f32
   */
  TIF_KTYPE_INT8_MIX_FP32 = 0b1000,
  /**
   * @brief Using int8 for each layer except that some layers which are
   * sensitive to precision will use f16, unsupported.
   */
  TIF_KTYPE_INT8_MIX_FP16 = 0b10000,
  /**
   * @brief Using uint8 for each layer except that some layers which are
   * sensitive to precision will use f32, unsupported.
   */
  TIF_KTYPE_UINT8_MIX_FP32 = 0b100000,
  /**
   * @brief Using uint8 for each layer except that some layers which are
   * sensitive to precision will use f16, unsupported.
   */
  TIF_KTYPE_UINT8_MIX_FP16 = 0b1000000,
  /**
   * @brief Refitting an engine with new weights without having to rebuild it
   *
   */
  TIF_REFIT = 0b1000000000000,
};

// doxygen end BuildFlag
/**
 * @}
 */

/**
 *  @defgroup CustomOpRegisterType
 *  @{
 *  This section describes the TopsInference CustomOpRegisterType.
 *
 */

/**
 * @brief The type of custom op registration. Compile the code of different
 * types to *.so file, and register the custom op with the *.so filepath.
 */
enum class CustomOpRegisterType {
  /**
   * @brief Using pure C/C++ code to implement custom op on CPU host. e.g.,
   * extern "C" void add(float* out, float* lhs, float* rhs) {
   *    for (int32_t i = 0; i < 16384; ++i)
   *        *(out+i) = *(lhs+i) + *(rhs+i);
   * }
   */
  CUSTOM_REGIST_PURE_C = 0,
  /**
   * @brief Using TopsOp library and APIs to implement custom op on GCU device.
   * e.g.,
   * extern "C" void add(Tensor* left, Tensor* right, Tensor* output,
   *            topsStream_t stream, Metadata* metadata) {
   *    ...
   *    topsopAdd(...);
   *    ...
   * }
   */
  CUSTOM_REGIST_TOPSOP = 1,
  /**
   * @brief Using topscc APIs to implement custom op on GCU device. (todo:
   * Currently the code should cover H2D and D2H due to the limitation of
   * topscc, which should be optimized in the future). e.g.,
   * extern "C" void add(float *out_h, float *lhs_h, float *rhs_h) {
   *    ... // H2D
   *    _my_add_kernel<<<1, 1>>>(out_d, lhs_d, rhs_d);
   *    ... // D2H
   * }
   */
  CUSTOM_REGIST_TOPSCC = 2,
  CUSTOM_REGIST_JITOP = 3,
};

// doxygen end CustomOpRegisterType
/**
 * @}
 */

/**
 *  @defgroup DataType
 *  @{
 *  This section describes the TopsInference DataType.
 *
 */

/**
 * @brief Data type definition for ITensor
 */
enum DataType {
  /**
   * @brief bool
   *
   */
  TIF_BOOL = 0,
  /**
   * @brief index
   *
   */
  TIF_INDEX,
  /**
   * @brief int8
   *
   */
  TIF_INT8,
  /**
   * @brief int16
   *
   */
  TIF_INT16,
  /**
   * @brief int32
   *
   */
  TIF_INT32,
  /**
   * @brief int64
   *
   */
  TIF_INT64,
  /**
   * @brief uint8
   *
   */
  TIF_UINT8,
  /**
   * @brief uint16
   *
   */
  TIF_UINT16,
  /**
   * @brief uint32
   *
   */
  TIF_UINT32,
  /**
   * @brief uint64
   *
   */
  TIF_UINT64,
  /**
   * @brief fp16
   *
   */
  TIF_FP16,
  /**
   * @brief fp32
   *
   */
  TIF_FP32,

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief fp64
   *
   */
  TIF_FP64,
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
  /**
   * @brief bf16
   *
   */
  TIF_BF16,
  /**
   * @brief tuple
   *
   */
  TIF_TUPLE,

  /**
   * @brief invalid
   *
   */
  TIF_INVALID
};

// doxygen end DataType
/**
 * @}
 */

/**
 *  @defgroup DataDeviceType
 *  @{
 *  This section describes the TopsInference DataDeviceType.
 *
 */

//!
//! The buffer type of the tensor.
//!
enum class DataDeviceType : int32_t {
  /**
   * @brief On HOST memory
   */
  HOST = 0,
  /**
   * @brief On GCU device memory
   */
  DEVICE
};

// doxygen end DataDeviceType
/**
 * @}
 */

/**
 *  @defgroup ParserType
 *  @{
 *  This section describes the TopsInference ParserType.
 *
 */

/**
 * @brief Parsed model type definition
 *
 */
enum ParserType {
  /**
   * @brief ONNX Model
   *
   */
  TIF_ONNX = 0,
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
   * @brief HLIR Graph
   *
   */
  TIF_EIR_GRAPH = 1
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
};

// doxygen end ParserType
/**
 * @}
 */

/**
 *  @defgroup MemcpyKind
 *  @{
 *  This section describes the TopsInference MemcpyKind.
 *
 */

/**
 * @brief Memory copy kind
 *
 */
enum MemcpyKind {
  /**
   * @brief Copy data from host to GCU device
   *
   */
  TIF_MEMCPY_HOST_TO_DEVICE = 0,
  /**
   * @brief Copy data from GCU device to host
   *
   */
  TIF_MEMCPY_DEVICE_TO_HOST = 1
};

// doxygen end MemcpyKind
/**
 * @}
 */

/**
 *  @defgroup BufferType
 *  @{
 *  This section describes the TopsInference BufferType.
 *
 */

/**
 * @brief Buffer type definition used in engine inference.
 *
 * Now, mixed buffer type is not supported, e.g. IN_HOST_OUT_DEVICE or
 * IN_DEVICE_OUT_HOST.
 *
 */
enum BufferType {
  /**
   * @brief The input buffer and output buffer is all allocated on host
   *
   */
  TIF_ENGINE_RSC_IN_HOST_OUT_HOST = 0,
  /**
   * @brief The input buffer and output buffer is all allocated on device
   *
   */
  TIF_ENGINE_RSC_IN_DEVICE_OUT_DEVICE = 1
};

// doxygen end BufferType
/**
 * @}
 */

/**
 *  @defgroup CalibrationAlgoType
 *  @{
 *  This section describes the TopsInference CalibrationAlgoType.
 *
 */

/**
 * enum CalibrationAlgoType
 *
 * @brief calibration algorithm to use.
 *
 */
enum class CalibrationAlgoType : int32_t {
  /**
   * @brief Invalid
   *
   */
  INVALID = -1,
  /**
   * @brief Kullback-Leibler Divergence Entropy / Relative Entropy
   *
   */
  KL_ENTROPY = 0,
  /**
   * @brief Max Min
   *
   */
  MAX_MIN = 1,
  /**
   * @brief Max Min EMA
   *
   */
  MAX_MIN_EMA = 2,
  /**
   * @brief Percentile
   *
   */
  PERCENTILE = 3,
};

/**
 * @brief Maximum number of elements in CalibrationAlgoType enum.
 *
 * @return in32_t
 */
template <>
constexpr inline int32_t EnumMax<CalibrationAlgoType>() noexcept {
  return 4;
}

// doxygen end CalibrationAlgoType
/**
 * @}
 */

// doxygen end Type
/**
 * @}
 */

/**
 *  @defgroup VersionInfo version information
 *  @{
 *  This section describes the TopsInference version information.
 *
 */

/**
 * @brief TopsInference version information
 *
 */
struct VersionInfo {
  // MAJOR version when making incompatible API or ABI changes
  uint32_t major;
  // MINOR version when adding functionality in a backward compatible manner
  uint32_t minor;
  // PATCH version when making backward compatible bug fixes
  uint32_t patch;
  // TWEAK version when making tiny change
  uint32_t tweak;
};

// doxygen end VersionInfo
/**
 * @}
 */

/**
 *  @defgroup Dims32 Dimension information
 *  @{
 *  This section describes the TopsInference Dimension information API.
 *
 */

/**
 * @brief Dimension information definition
 *
 */
class TOPS_INFERENCE_EXPORT Dims32 {
 public:
  /**
   * @brief The max dimension supported by the class
   *
   */
  static const int32_t MAX_DIMS = 8;
  /**
   * @brief The actual dimension
   *
   */
  int32_t nbDims = 0;
  /**
   * @brief The size in each dimension
   *
   */
  int32_t dimension[MAX_DIMS] = {0};
};

//!
//! Alias for Dims32.
//!
//! \warning: This alias might change in the future.
//!
using Dims = Dims32;

// doxygen end Dims32
/**
 * @}
 */

/**
 *  @defgroup ITensor ITensor
 *  @{
 *  This section describes the TopsInference ITensor API.
 *
 */

/**
 * @brief ITensor information definition
 * The attributes of input and output, including origin data pointer, shape,
 * device type, etc., are recorded in ITensor.
 *
 */
class TOPS_INFERENCE_EXPORT ITensor {
 public:
  virtual ~ITensor() {}
  /**
   * @brief Get the data pointer of the ITensor.
   * @return The data pointer.
   */
  virtual void* getOpaque() = 0;
  /**
   * @brief Set the data pointer of the ITensor.
   * @param opaque The data pointer. If using DeviceType as HOST, opaque
   * represent HOST memory pointer, if using DeviceType as Device, opaque
   * should use pointer of DeviceMemory
   */
  virtual bool setOpaque(void* opaque) = 0;

  /**
   * @brief Get the dimension.
   * @return Dimension array.
   */
  virtual Dims getDims() = 0;
  /**
   * @brief Set dimension.
   * @param dims The dimension.
   */
  virtual void setDims(Dims dims) = 0;
  /**
   * @brief  Get the device type of the ITensor buffer(@see DataDeviceType).
   * @return The device type of the ITensor buffer.
   */
  virtual DataDeviceType getDeviceType() = 0;
  /**
   * @brief Set the device type of the ITensor buffer(@see DataDeviceType).
   * @param device_type The device type of the ITensor buffer.
   */
  virtual void setDeviceType(DataDeviceType device_type) = 0;

  /**
   * @brief Release current ITensor, and this interface must be called once you
   * want to delete the ITensor.
   */
  virtual void release() = 0;

  /**
   * @brief  Get the type of the ITensor buffer(@see DataType).
   * @return The type of the ITensor buffer.
   */
  virtual DataType getDataType() = 0;
  /**
   * @brief  Set the type of the ITensor buffer(@see DataType).
   * @return The type of the ITensor buffer.
   */
  virtual void setDataType(DataType datatype) = 0;
};

typedef class ITensor* TensorPtr_t;

// doxygen end ITensor
/**
 * @}
 */

/**
 *  @defgroup IErrorManager error manager information
 *  @{
 *  This section describes the TopsInference error manager information API.
 *
 */

/**
 * @brief Error Manager for recording the internal errors
 * @warning Which is thread-safe
 * @note Examples:
 * @code
        IParser *onnx_parser = create_parser(ParserType::TIF_ONNX);
        IErrorManager *error_manager = create_error_manager();

        try {
          const char *model = "add.onnx";
          INetwork *network = onnx_parser->readModel(model);
          if (network == nullptr) {
            int32_t error_count = error_manager->getErrorCount();
            for (int32_t i = 0; i < error_count; i++) {
              char* error_msg = error_manager->getErrorMsg(i);
              TIFStatus error_status = error_manager->getErrorStatus(i);
            }
            error_manager->clear();
          }
        } catch (std::exception &e) {
          int32_t error_count = error_manager->getErrorCount();
          for (int32_t i = 0; i < error_count; i++) {
            char* error_msg = error_manager->getErrorMsg(i);
            TIFStatus error_status = error_manager->getErrorStatus(i);
        }
          error_manager->clear();
        }
 * @endcode
 */
class TOPS_INFERENCE_EXPORT IErrorManager {
 public:
  virtual ~IErrorManager() {}
  /**
   * @brief Get the Error Status
   *
   * @param index The index for queried error, index < getErrorCount()
   * @return TIFStatus @see TIFStatus
   */
  virtual TIFStatus getErrorStatus(int32_t index) = 0;
  /**
   * @brief Get the Error Count object
   *
   * @return int32_t The error count in error manager
   */
  virtual int32_t getErrorCount() = 0;
  /**
   * @brief Get the Error Msg object
   *
   * @param index The index for queried error, index < getErrorCount()
   * @return const char* The error message
   */
  virtual const char* getErrorMsg(int32_t index) = 0;
  /**
   * @brief print the Error Status and Msg as String
   *
   * @param index The index for queried error, index < getErrorCount()
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool reportStatus(int32_t index) = 0;
  /**
   * @brief Need to call after finishing to query
   *
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool clear() = 0;
};

// doxygen end IErrorManager
/**
 * @}
 */

/**
 *  @defgroup IConfig config
 *  @{
 *  This section describes the TopsInference Config API.
 *
 */

/**
 * @brief Common config definition
 *
 */
class TOPS_INFERENCE_EXPORT IConfig {
 public:
  virtual ~IConfig() {}
  /**
   * @brief Load config from proto buffer file name
   *
   * @param proto
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool loadConfig(const char* proto) = 0;
  /**
   * @brief Save config into proto buffer file name
   *
   * @param proto
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool saveConfig(const char* proto) = 0;
};

/**
 * @brief Config for IParser
 *
 */
class TOPS_INFERENCE_EXPORT IParserConfig : public IConfig {
 public:
  virtual ~IParserConfig() {}
  /**
   * @brief Simplify the network when parsing
   *
   * @param simplify If True, to simplify the network
   */
  virtual void setSimplify(bool simplify) = 0;
  /**
   * @brief Check the simplify status in parser
   *
   * @return bool
   * @retval true. Config is simplify
   * @retval false. Config is not simplify
   */
  virtual bool getSimplify() = 0;
};

/**
 * @brief Config for Optimizer
 *
 */
class TOPS_INFERENCE_EXPORT IOptimizerConfig : public IConfig {
 public:
  virtual ~IOptimizerConfig() {}
  /**
   * @brief Set the Compile Options
   * @param options is json format
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setCompileOptions(const char* options) = 0;
  /**
   * @brief Get the compile options
   *
   * @return const char* Current compile options
   */
  virtual const char* getCompileOptions() const = 0;
  /**
   * @brief Set the build flag
   *
   * @param flag @see BuildFlag
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setBuildFlag(BuildFlag flag) = 0;
  /**
   * @brief Set the build flag
   *
   * @param flag formed by composing multiple BuildFlag
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setBuildFlag(int64_t flag) = 0;
  /**
   * @brief Get the build flag
   *
   * @return int64_t BuildFlag
   */
  virtual int64_t getBuildFlag() = 0;
  /**
   * @brief set the max shape dim for the specified op with json string
   * @param op_shape is json format
   * @note Examples:
   * @code
        Json::Value max_shape_range_setting;
        Json::Value op_max_val;
        op_max_val["main"].append("1,3,512,512");
        max_shape_range_setting.append(op_max_val);
        Json::Value min_shape_range_setting;
        Json::Value op_min_val;
        op_min_val["main"].append("1,3,112,112");
        min_shape_range_setting.append(op_min_val);
        char* max_setting_str = max_shape_range_setting.toStyledString();
        char* min_setting_str = min_shape_range_setting.toStyledString();
        assert((optimizer_config->setMaxShapeRange(max_setting_str.c_str()),
            "[Error] set max shape range failed!"));
        assert((optimizer_config->setMinShapeRange(min_setting_str.c_str()),
            "[Error] set min shape range failed!"));
   * @endcode
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setMaxShapeRange(const char* op_shape) = 0;
  /**
   * @brief set the min shape dim for the specified op with json string
   * @param op_shape is json format  @see setMaxShapeRange
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setMinShapeRange(const char* op_shape) = 0;
  /**
   * @brief config the calibrator
   * @param calibrator a pointer to the calibrator
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setInt8Calibrator(ICalibrator* calibrator) = 0;
  /**
   * @brief Set the refit preprocess mode.
   *
   * @param refit_preprocess_flag If true, enable refit with preprocess.
   */
  virtual void setRefitPreprocess(bool refit_preprocess_flag) = 0;
};

/**
 * @brief Config for engine
 *
 */
class TOPS_INFERENCE_EXPORT IEngineConfig : public IConfig {
 public:
  virtual ~IEngineConfig() {}
  /**
   * @brief Get the engine version
   *
   * @return const char* engine version
   */
  virtual const char* getEngineVersion() = 0;
  /**
   * @brief Set the auto batch mode.
   *
   * @param auto_mode If true, set auto batch mode
   */
  virtual void setAutoBatchMode(bool auto_mode) = 0;
  /**
   * @brief Get the auto batch mode.
   *
   * @return bool
   * @retval true. Config is auto batch mode
   * @retval false. Config is not auto batch mode
   */
  virtual bool isAutoBatchMode() = 0;
};

/**
 * @brief Config for Calibrator
 *
 */
class TOPS_INFERENCE_EXPORT ICalibratorConfig : public IConfig {
 public:
  virtual ~ICalibratorConfig() {}
  /**
   * @brief set the op precision used for calibration
   *
   * @param op_name op name to be set
   * @param dtype set the op to dtype precision
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setOpPrecision(const char* op_name, DataType dtype) = 0;

  /**
   * @brief set the op algorithm used for calibration
   *
   * @param op_name op name to be set
   * @param algo set the op to algo algorithm
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setOpCalibrateAlgo(const char* op_name,
                                  CalibrationAlgoType algo) = 0;
  /**
   * @brief set the op threshold for calibration
   *
   * @param op_name op name to be set
   * @param thres_val set the op to thres_val threshold
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setOpThreshold(const char* op_name, double thres_val) = 0;

  /**
   * @brief set the op types to be quantized
   *
   * @param op_types op types to be set
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool enableQuantizeOps(const char** op_types, int32_t num) = 0;
  /**
   * @brief get the op type which will be quantized
   *
   * @param op_types op types to be quantized
   * @param num the num of quantized op_type
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool getQuantizeOps(char** op_types, int32_t* num) = 0;

  /**
   * @brief exclude the op types which will be quantized
   *
   * @param op_types op types to be set
   * @param num the num of quantized op_type
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool disableQuantizeOps(const char** op_types, int32_t num) = 0;

  /**
   * @brief get the op precision set in calibration
   *
   * @param op_name op name
   * @return DataType op precision
   */
  virtual DataType getOpPrecision(const char* op_name) = 0;

  /**
   * @brief get the op algorithm set in calibration
   *
   * @param op_name op name
   * @return CalibrationAlgoType op algorithm
   */
  virtual CalibrationAlgoType getOpCalibrateAlgo(const char* op_name) = 0;

  /**
   * @brief get the op threshold set in calibration
   *
   * @param op_name op name
   * @return double op threshold
   */
  virtual double getOpThreshold(const char* op_name) = 0;
};

// doxygen end IConfig
/**
 * @}
 */

/**
 * @addtogroup Type
 * @{
 *
 */

/**
 *  @defgroup LayerType
 *  @{
 *  This section describes the TopsInference LayerType.
 *
 */

/**
 * @brief Layer type definition used in ILayer.
 *
 *
 */
enum LayerType {
  /**
   * @brief DeConvolution layer.
   *
   */
  TIF_DECONVOLUTION = 0,
  /**
   * @brief Convolution layer.
   *
   */
  TIF_CONVOLUTION,
  /**
   * @brief UnaryOp operation layer.
   *
   */
  TIF_UNARY,
  /**
   * @brief Transcendental layer.
   *
   */
  TIF_TRANSCENDENTAL,
  /**
   * @brief Elementwise operation layer.
   *
   */
  TIF_ELEMENTWISE,
  /**
   * @brief Select layer.
   *
   */
  TIF_SELECT,
  /**
   * @brief Pooling layer.
   *
   */
  TIF_POOLING,
  /**
   * @brief Batch normalization layer.
   *
   */
  TIF_BATCHNORM,
  /**
   * @brief Convert layer for converting between different data precision.
   *
   */
  TIF_CONVERT,
  /**
   * @brief Concat layer.
   *
   */
  TIF_CONCAT,
  /**
   * @brief Constant layer.
   *
   */
  TIF_CONSTANT,
  /**
   * @brief Shuffle layer.
   *
   */
  TIF_SHUFFLE,
  /**
   * @brief Activation layer.
   *
   */
  TIF_ACTIVATION,
  /**
   * @brief Layer for sorting by a certain rule.
   *
   */
  TIF_ORDER,
  /**
   * @brief Rnn layer.
   *
   */
  TIF_RNN,
  /**
   * @brief Gather layer.
   *
   */
  TIF_GATHER,
  /**
   * @brief MatMul layer.
   *
   */
  TIF_MATMUL,
  /**
   * @brief Compare layer.
   *
   */
  TIF_COMPARE,
  /**
   * @brief Condition layer.
   *
   */
  TIF_CONDITION,
  /**
   * @brief Non max suppression layer.
   *
   */
  TIF_NMS,
  /**
   * @brief Padding layer.
   *
   */
  TIF_PAD,
  /**
   * @brief Random generator layer.
   *
   */
  TIF_RANDOM,
  /**
   * @brief Reduce layer.
   *
   */
  TIF_REDUCE,
  /**
   * @brief Reshape layer.
   *
   */
  TIF_RESHAPE,
  /**
   * @brief Resize layer.
   *
   */
  TIF_RESIZE,
  /**
   * @brief ROI align layer.
   *
   */
  TIF_ROIALIGN,
  /**
   * @brief Scatter layer.
   *
   */
  TIF_SCATTER,
  /**
   * @brief Sigmoid layer.
   *
   */
  TIF_SIGMOID,
  /**
   * @brief Slice layer.
   *
   */
  TIF_SLICE,
  /**
   * @brief TopK layer.
   *
   */
  TIF_TOPK,
  /**
   * @brief Transpose layer.
   *
   */
  TIF_TRANSPOSE,
  /**
   * @brief Mean-variance normalization layer.
   *
   */
  TIF_MVN,
  /**
   * @brief Softmax layer.
   *
   */
  TIF_SOFTMAX,
  /**
   * @brief Log softmax layer.
   *
   */
  TIF_LOG_SOFTMAX,
  /**
   * @brief Sample layer.
   *
   */
  TIF_SAMPLE,
  /**
   * @brief Dequantize layer.
   *
   */
  TIF_DEQUANTIZE,
  /**
   * @brief Cumsum layer.
   *
   */
  TIF_CUMSUM,
  /**
   * @brief DTF(discrete Fourier transform) layer.
   *
   */
  TIF_DFT,
  /**
   * @brief DET layer.
   *
   */
  TIF_DET,
  /**
   * @brief einsum layer.
   *
   */
  TIF_EINSUM,
  /**
   * @brief loop layer.
   *
   */
  TIF_LOOP,
  /**
   * @brief mel weight matrix layer.
   *
   */
  TIF_MELWEIGHT,
  /**
   * @brief optinal serise op layer.
   *
   */
  TIF_OPTIONAL,
  /**
   * @brief Quantize layer.
   *
   */
  TIF_QUANTIZE,
  /**
   * @brief sequence serise op layer.
   *
   */
  TIF_SEQUENCE,
  /**
   * @brief STFT(Short-time Fourier Transform) layer.
   *
   */
  TIF_STFT,
  /**
   * @brief TFIDF layer.
   *
   */
  TIF_TFIDF,
  /**
   * @brief TRILU(triangular) layer.
   *
   */
  TIF_TRILU,
  /**
   * @brief STRINGNORM layer.
   *
   */
  TIF_STRINGNORM,
  /**
   * @brief Expand layer.
   *
   */
  TIF_EXPAND,
  /**
   * @brief Customcall layer.
   *
   */
  TIF_CUSTOM,
  /**
   * @brief Normlization layer.
   *
   */
  TIF_NORMLIZE,
  /**
   * @brief map layer.
   *
   */
  TIF_MAP,
  /**
   * @brief tile layer.
   *
   */
  TIF_TILE,
  /**
   * @brief dropout layer.
   *
   */
  TIF_DROPOUT,
  /**
   * @brief structural layer.
   *
   */
  TIF_STRUCTURAL,
};

// doxygen end LayerType
/**
 * @}
 */

// doxygen end Type
/**
 * @}
 */

/**
 *  @defgroup ILayer Layer information
 *  @{
 *  This section describes the TopsInference Layer information API.
 *
 */

/**
 * @brief Layer definition.
 * Base class for all layer classes in a network definition.
 *
 */
class TOPS_INFERENCE_EXPORT ILayer {
 public:
  virtual ~ILayer() {}
  /**
   * @brief Get the layer type
   *
   * @return LayerType @see LayerType
   */
  virtual LayerType getType() = 0;
  /**
   * @brief Set the layer name, take effect at compile time
   *
   * @param name layer name
   */
  virtual void setName(const char* name) = 0;
  /**
   * @brief Get the layer name
   *
   * @return const char*
   */
  virtual const char* getName() = 0;
  /**
   * @brief Set the layer precision, take effect at compile time.
   * In TIF_KTYPE_MIX_FP16 mode, user can set TIF_FP32 or TIF_FP16.
   * In TIF_KTYPE_INT8_MIX_FP32 mode, user can set TIF_FP32 or TIF_INT8.
   *
   * @param dataType set DataType::TIF_FP32 or DataType::TIF_FP16 or
   * DataType::TIF_INT8
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool setPrecision(DataType dataType) = 0;
  /**
   * @brief Get the layer precision.
   *
   * @return DataType
   */
  virtual DataType getPrecision() = 0;
  /**
   * @brief Check whether the layer precision is set or not.
   *
   * @return bool whether the precision has been set
   */
  virtual bool isPrecisionSet() = 0;
  /**
   * @brief Reset the layer precision, take effect at compile time
   *
   */
  virtual void resetPrecision() = 0;
};

// doxygen end ILayer
/**
 * @}
 */

/**
 *  @defgroup INetwork Network definition
 *  @{
 *  This section describes the TopsInference Network definition API.
 *
 */

/**
 * @brief Network definition
 * A network definition to the builder
 *
 */
class TOPS_INFERENCE_EXPORT INetwork {
 public:
  virtual ~INetwork() {}
  /**
   * @brief Get the layer number
   *
   * @return int32_t layer number
   */
  virtual int32_t getLayerNum() = 0;
  /**
   * @brief Get the layer according to the index,
   *
   * @param index the index must be less than the layer number
   * @return ILayer* Ilayer pointer
   */
  virtual ILayer* getLayer(int32_t index) = 0;
  /**
   * @brief Dump the network information
   *
   */
  virtual void dump() = 0;
  /**
   * @brief Get the layer according to the layer name,
   * also support fuzzy match by regex expression
   *
   * @param regex_str regex expression
   * @param match_num matched layer number
   * @return ILayer* Ilayer pointer
   */
  virtual ILayer** getLayer(const char* regex_str, int32_t* match_num) = 0;
};

// doxygen end INetwork
/**
 * @}
 */

/**
 *  @defgroup IParser Parser definition
 *  @{
 *  This section describes the TopsInference Parser definition API.
 *
 */

/**
 * @brief Parser definition
 * IParser is a compiler component that translate onnx model into
 * TopsInference network definition.
 *
 */
class TOPS_INFERENCE_EXPORT IParser {
 public:
  virtual ~IParser() {}
  /**
   * @brief Read model file, support onnx file
   *
   * @param model model file
   * @return INetwork*  @see{INetwork}
   */
  virtual INetwork* readModel(const char* model) = 0;
  /**
   * @brief Read model string
   *
   * @param model model string
   * @param model_size model size
   * @return INetwork*  @see{INetwork}
   */
  virtual INetwork* readModelFromStr(const char* model,
                                     uint32_t model_size) = 0;
  /**
   * @brief Read model object.
   * This interface is not ABI compatibility. Please use abi1 onnx,
   * compiled with _GLIBCXX_USE_CXX11_ABI=1, which is the default value
   * for gcc with version numbers greater than 5.
   *
   * @param model model object
   * @return INetwork*  @see{INetwork}
   */
  virtual INetwork* readModelObj(const void* model_obj) = 0;
  /**
   * @brief Get the model
   *
   * @return INetwork*
   */
  virtual INetwork* getModel() = 0;
  /**
   * \deprecated Deprecated in TopsInference 2.5.
   */
  TIF_DEPRECATED virtual void setDebugInfoFile(const char* val) = 0;
  /**
   * @brief Set the input names before reading model, when there are multi
   * inputs, names are separated with a comma, such as "a,b". If not set,
   * the original properties of the network are used.
   *
   * @param val input names
   */
  virtual void setInputNames(const char* val) = 0;
  /**
   * @brief Set the input data types before reading model, when there are multi
   * inputs, types are separated with a comma, such as "TIF_FP32,TIF_FP32".
   * Subnets of the network can be intercepted after setting the input and
   * output. If not set, the original properties of the network are used.
   * @param val input dtypes
   */
  virtual void setInputDtypes(const char* val) = 0;
  /**
   * @brief Set the input shapes before reading model, when there are multi
   * inputs, shapes are separated with a colon, such as "3,4:3,4". If not
   * set, the original properties of the network are used.
   *
   * @param val input shapes
   */
  virtual void setInputShapes(const char* val) = 0;
  /**
   * @brief Set the output names before reading model, when there are multi
   * outputs, names are separated with a comma, such as "a,b". If not set,
   * the original properties of the network are used.
   *
   * @param val output names
   */
  virtual void setOutputNames(const char* val) = 0;

  /**
   * @brief Set the output data types before reading model, when there are multi
   * outputs, types are separated with a comma, such as "TIF_FP32,TIF_FP32".
   * Subnets of the network can be intercepted after setting the input and
   * output. If not set, the original properties of the network are used.
   *
   * @param val output dtypes
   */
  virtual void setOutputDtypes(const char* val) = 0;

  /**
   * @brief Get the config pointer used for parser, use IParserConfig*-> to set
   * @return IParserConfig*
   */
  virtual IParserConfig* getConfig() = 0;
};

// doxygen end IParser
/**
 * @}
 */

/**
 *  @defgroup IStream Stream definition
 *  @{
 *  This section describes the TopsInference Stream definition API.
 *
 */

/**
 * @brief Stream definition
 * Stream of asynchronous action
 *
 */
class TOPS_INFERENCE_EXPORT IStream {
 public:
  virtual ~IStream() {}
  /**
   * @brief Stream synchronize, Wait for any pending asynchronous action in
   * stream
   *
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool synchronize() = 0;
};

using topsInferStream_t = IStream*;

// doxygen end IStream
/**
 * @}
 */

/**
 *  @defgroup IFuture Future definition
 *  @{
 *  This section describes the TopsInference Future definition API.
 *
 */

/**
 * @brief IFuture is used for asynchronous inference, Used to describe
 * whether the current output data status is ready
 */
class TOPS_INFERENCE_EXPORT IFuture {
 public:
  virtual ~IFuture() {}
  /**
   * @brief wait until output data is ready
   */
  virtual void wait() = 0;
  /**
   * @brief status of output data
   * @return if output data was ready, return true, otherwise return false
   */
  virtual bool status() = 0;
};

// doxygen end IFuture
/**
 * @}
 */

/**
 *  @defgroup IEngine Engine definition
 *  @{
 *  This section describes the TopsInference Engine definition API.
 *
 */

/**
 * @brief Executable definition, the serialized engine contains the necessary
 * copies of the weights, the parser, network definition   @see IOptimizer
 *
 */
class TOPS_INFERENCE_EXPORT IEngine {
 public:
  virtual ~IEngine() {}
  virtual bool release() = 0;

  /**
   * @brief Save engine to file
   *
   * @param name Engine file
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool saveExecutable(const char* name) = 0;
  /**
   * @brief Load engine from file
   *
   * @param name Engine file
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool loadExecutable(const char* name) = 0;

  /**
   * @brief Deserialize an engine from buffer
   * @param blob  The memory that holds the serialized executable
   * @param size  The size of the memory
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool loadExecFromBuffer(const void* blob, std::size_t size) = 0;

  /**
   * @brief Get the set input shape corresponding to the given index, the index
   * argument must be less than the value of getInputNum(). if using dynamic
   * shape mode, Dims may contain -1
   * @param index Each engine may have several input nodes, The index-th input
   * @return Dims
   */
  virtual Dims getInputShape(int32_t index) = 0;
  /**
   * @brief Get the output shape corresponding to the given index, the index
   * argument must be less than the value of getOutputNum()
   *
   * @param index Each engine may have several output nodes, The index-th
   * output.
   * @return Dims
   */
  virtual Dims getOutputShape(int32_t index) = 0;
  /**
   * @brief Get the input num
   *
   * @return int32_t
   */
  virtual int32_t getInputNum() = 0;
  /**
   * @brief Get the output num
   *
   * @return int32_t
   */
  virtual int32_t getOutputNum() = 0;
  /**
   * @brief Get the input data type corresponding to the given index, the index
   * argument must be less than the value of getInputNum()
   *
   * @param index Each engine may have several input nodes, The index-th input
   * @return DataType
   */
  virtual DataType getInputDataType(int32_t index) = 0;
  /**
   * @brief Get the output data type corresponding to the given index, the index
   * argument must be less than the value of getOutputNum()
   *
   * @param index Each engine may have several output nodes, The index-th
   * output
   * @return DataType
   */
  virtual DataType getOutputDataType(int32_t index) = 0;

  /**
   * @brief Get the maximum input shape by index,
   * this interface is only used when the current engine's index-th input has
   * dynamic shape. if used in static shape mode, it will return static shape.
   * @param index The input id, the index of the input
   * @return The maximum input dimension
   */
  virtual Dims getMaxInputShape(uint32_t index) = 0;
  /**
   * @brief Get the maximum output shape by index,
   * this interface is only used when the current engine's index-th output has
   * dynamic or unknown shape
   * @param index The output id, the index of the output
   * @return The maximum output shape, if the maximum input shape has not been
   * set when compiling, it will raise error
   */
  virtual Dims getMaxOutputShape(uint32_t index) = 0;
  /**
   * @brief Get the minimum input shape by index,
   * this interface is only used when the current engine's index-th input has
   * dynamic shape. if used in static shape mode, it will return static shape.
   * @param index The input id, the index of the input
   * @return The minimum input dimension.
   */
  virtual Dims getMinInputShape(uint32_t index) = 0;

  /**
   * @brief get the IEngine Config pointer, use IEngineConfig*-> to set
   *
   * @return IEngineConfig*
   */
  virtual IEngineConfig* getConfig() = 0;
  /**
   * @brief run with specified device(cluster)
   *
   * @param input The input buffer bound for current engine, data is arranged
   * in Column-major order
   * @param output The output buffer bound for current engine, data is arranged
   * in Column-major order
   * @param buf_type engine run mode @see BufferType
   * @param stream when stream is not nullptr, asynchronously
   *
   * When running with auto batch mode, the async mode is not supported now, it
   * means that you must keep stream nullptr
   *
   * When doing inference with buf_type equal to IN_HOST_OUT_HOST, the async
   * mode is not supported now, it means that you * must keep stream nullptr
   *
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool run(void** input, void** output, BufferType buf_type,
                   topsInferStream_t stream = nullptr) = 0;

  /**
   * @brief run with specified device(cluster) and dynamic batch
   *
   * @param sample_num sample nums of runtime batch size
   * @param input The sample_num input buffer bound for current engine, data is
   * arranged in Column-major order, , [[sample1_input_1, sample2_input_1, ... ,
   * sample_x_input_1],[sample1_input_2, sample2_input_2, ... ,
   * sample_x_input_2],..
   * @param output The sample_num output buffer bound for current engine, data
   * is arranged in Column-major order
   * @param buf_type engine run mode @see BufferType
   * @param stream when stream is not nullptr, run asynchronously
   * @param future Future is an optional parameter,When you are using
   * asynchronous mode and you want to know the current request status, please
   * use it
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool runWithBatch(std::size_t sample_num, void** inputs,
                            void** outputs, BufferType buf_type,
                            topsInferStream_t stream = nullptr,
                            IFuture* future = nullptr) = 0;

  /**
   * @brief run with specified device(cluster) and dynamic batch
   *
   * @param inputs The input tensor list pointer for each input
   * @param outputs The output tensor list pointer for each output
   * @param stream When stream is not null, run with asynchronous mode,
   * otherwise with synchronous mode
   * @param future Future is an optional parameter, please keep it not null When
   * you want to know the current request status with asynchronous mode
   * @return TIFStatus @see TIFStatus
   */
  virtual TIFStatus runV2(IN TensorPtr_t* inputs, INOUT TensorPtr_t* outputs,
                          topsInferStream_t stream = nullptr,
                          IFuture* future = nullptr) = 0;

  /**
   * @brief run with specified device(cluster) and dynamic batch,
   * and this interface can be mixed with rt3.0
   *
   * @param inputs The input tensor list pointer for each input
   * @param outputs The output tensor list pointer for each output
   * @param stream When stream is not null, run with asynchronous mode,
   * otherwise with synchronous mode
   * @param future Future is an optional parameter, please keep it not null When
   * you want to know the current request status with asynchronous mode
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool runV3(IN TensorPtr_t* inputs, INOUT TensorPtr_t* outputs,
                     topsStream_t stream = nullptr,
                     IFuture* future = nullptr) = 0;

  /**
   * @brief Get the input name by index. the index
   * argument must be less than the value of getInputNum()
   * The life cycle of the pointer is consistent with that
   * of the engine, The value may change in reload or rebuild.
   * @param index The input id, the index of the input
   * @return The index-th input name.
   */
  virtual const char* getInputName(uint32_t index) = 0;

  /**
   * @brief Get the output name by index. the index
   * argument must be less than the value of getOutputNum()
   * The life cycle of the pointer is consistent with that
   * of the engine, The value may change in reload or rebuild.
   * @param index The output id, the index of the output
   * @return The index-th output name.
   */
  virtual const char* getOutputName(uint32_t index) = 0;

  /**
   * @brief Get the memory size of gcu device runtime required.
   *
   * @return The engine's memory size required.
   *         Return 0 if fail to get memory size.
   */
  virtual size_t getDeviceMemorySize() = 0;

  /**
   * @brief run with specified device(cluster) and dynamic batch
   * If the model input is a dynamic shape, this API can help
   * push out the output shape
   * @param inputs The input tensor list pointer for each input
   * @param outputs The output tensor list pointer for each output
   * @return bool if success return true, otherwise return false
   */
  virtual bool shapeInfer(IN const TensorPtr_t* inputs,
                          INOUT TensorPtr_t* outputs) = 0;

  /**
   * @brief Set the N-dimensional information for input and output tensors.
   * If a dimension exists, specify the index of N in that dimension. If it
   * doesn't exist, set index to -1. It's not thread safety. If you
   * do not set the N index information after creating the engine, the N index
   *  for both inputs and outputs will default to 0, means NHWC/NCHW/NWHC/NXXX
   * formats.
   * @param inputNIndices The input tensor list pointer for each input
   * @param outputNIndices The output tensor list pointer for each output
   * @return bool if success return true, otherwise return false
   */
  virtual bool setIODimensionInfo(const int32_t* inputNIndices,
                                  const int32_t* outputNIndices) {
    return 0;
  }

  /**
  * @brief Set the maximum workspace size.
  * @param workspaceSize The maximum GCU temporary memory which the engine can
  * use at execution time.
  * @see getMaxWorkspaceSize()
  **/
  virtual void setMaxWorkspaceSize(std::size_t workspaceSize) noexcept {}
  /**
  * @brief Get the maximum workspace size.
  * By default the workspace size is the size of total global memory in the
  *device.
  * @return The maximum workspace size.
  * @see setMaxWorkspaceSize()
  **/
  virtual std::size_t getMaxWorkspaceSize() const
      noexcept {
    return 0;
  }
};

// doxygen end IEngine
/**
 * @}
 */

/**
 *  @defgroup IOptimizer Optimizer definition
 *  @{
 *  This section describes the TopsInference Optimizer definition API.
 *
 */

/**
 * @brief Optimizer definition
 * The optimizer will do a series of optimizations on the layer.
 *
 */
class TOPS_INFERENCE_EXPORT IOptimizer {
 public:
  virtual ~IOptimizer() {}
  virtual bool release() = 0;
  /**
   * @brief Build an engine from network
   *
   * @param network INetwork @see INetwork
   * @return IEngine* pointer of IEngine @see IEngine
   * @note Examples:
   * @code
    TopsInference::IEngine* engine = optimizer->build(network);
   * @endcode
   */
  virtual IEngine* build(INetwork* network) = 0;
  /**
   * @brief Get the config pointer used for optimizer  @see BuildFlag
   * use IOptimizerConfig*-> to set
   *
   * @return IOptimizerConfig*
   */
  virtual IOptimizerConfig* getConfig() = 0;
};

// doxygen end IOptimizer
/**
 * @}
 */

/**
 *  @defgroup Weights Weights definition
 *  @{
 *  This section describes the TopsInference Weights definition API.
 *
 */

/**
 * @brief An array of weights used as a layer parameter.
 * The weights are held by reference until the engine has been built. Therefore
 * the data referenced by values field should be preserved until the build is
 * complete
 */
struct TOPS_INFERENCE_EXPORT Weights {
  /**
   * @brief The data type of the weights
   */
  DataType type;
  /**
   * @brief The number of weights in the array
   */
  int64_t count;
  /**
   * @brief The weight values
   */
  const void* values;
  /**
   * @brief The weights dims
   *
   */
  Dims dims;
};

// doxygen end Weights
/**
 * @}
 */

/**
 *  @defgroup IRefitter Refitter definition
 *  @{
 *  This section describes the TopsInference refit definition API.
 *
 */

/**
 * @brief Updates weights in an engine.
 */
class TOPS_INFERENCE_EXPORT IRefitter {
 public:
  virtual ~IRefitter() {}
  /**
   * @brief Get names of all weights that could be refit.
   * @param [in] size The number of weights names that can be safely written
   * to.
   * @param [out] weightsNames The names of the weights to be updated, or
   * nullptr for unnamed weights.
   * @return The number of Weights that could be refit.
   * It should be call twice. The first use of getAllWeights(0, nullptr)
   * returns size, initializes the weightsNames with the size, and
   * passes in the size and weightsNames to get all weights
   */
  virtual int32_t getAllWeights(int32_t size, const char** weightsNames) = 0;
  /**
   * @brief Get names of missing weights.
   * For example, if some Weights have been set, but the engine was optimized in
   * a way that combines weights, any unsupplied Weights in the combination are
   * considered missing.
   * @param [in] size The number of weights names that can be safely written
   * to.
   * @param [out] weightsNames The names of the weights to be updated, or
   * nullptr for unnamed weights.
   * @return The number of missing Weights.
   * It should be call twice. The first use of getMissingWeights(0, nullptr)
   * returns size, initializes the weightsNames with the size, and
   * passes in the size and weightsNames to get missing weights
   */
  virtual int32_t getMissingWeights(int32_t size,
                                    const char** weightsNames) = 0;
  /**
   * @brief Specify new weights of given name
   * @param [in] name The name of the weights to be updated
   * @param [in] weights new weight
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if new weights are rejected
   */
  virtual bool setNamedWeights(const char* name, Weights weights) = 0;
  /**
   * @brief Obtain weights of given name.
   * @param [in] name The name of the weights to be obtain
   * @param [inout] weights weights must be initialized with type @see Weights
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if new weights are rejected
   */
  virtual bool getNamedWeights(const char* name, Weights& weights) = 0;
  /**
   * @brief Updates associated engine
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool refitEngine() = 0;
  /**
   * @brief Check whether the current refitter supports automatic preprocessing.
   */
  virtual bool isSupportPreprocess() = 0;
};

// doxygen end IRefitter
/**
 * @}
 */

/**
 *  @defgroup ICalibrator Calibrator definition
 *  @{
 *  This section describes the TopsInference Calibrator definition API.
 *
 */

/**
 * @class  IInt8Calibrator
 * @brief Application-implemented interface for calibration.
 */
class TOPS_INFERENCE_EXPORT ICalibrator {
 public:
  virtual ~ICalibrator() noexcept = default;
  /**
   * @brief Get the batch size used for calibration batches
   * @return The batch size
   */
  virtual int32_t getBatchSize() const noexcept = 0;
  /**
   * @brief Get a batch of input for calibration
   * The batch size of the input must match the batch size returned
   * by getBatchSize()
   * @param data An array of pointers to host memory that containing each
   * network input data
   * @param names The names of the network input for each pointer in the
   * binding array
   * @param num The number of pointers in the binding array
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if there are no more batches for calibration
   */
  virtual bool getBatch(TensorPtr_t data[], const char* names[],
                        int32_t num) noexcept = 0;

  /**
   * @brief  Load a calibration cache. Calibration is potentially expensive,
   * so it can be useful to generate the calibration data once, then use it on
   * subsequent builds of the network. The cache includes the regression cutoff
   * and quantized values used to generate it, and will not be used if these do
   * not batch the settings of the current calibrator. However, the network
   * should also be recalibrated if its structure changes, or the input data
   * set changes, and it is the responsibility of the application to ensure
   * this.
   *
   * @param length The length of the cached data, If there is no data,this
   * should be zero
   * @return A pointer to the cache, or nullptr if there is no data
   */
  virtual const void* readCalibrationCache(int64_t& length);
  /**
   * @brief Save a calibration cache
   *
   * @param ptr A pointer to the data to cache
   * @param length The length in bytes of the data to cache
   * @return bool
   * @retval true Return true if succeed
   * @retval false Return false if fail
   */
  virtual bool writeCalibrationCache(const void* ptr, int64_t length);
  /**
   * @brief Get the algorithm used by this calibrator.
   *
   * @return CalibrationAlgoType The algorithm used by the calibrator.
   */
  virtual CalibrationAlgoType getAlgorithm() noexcept = 0;

  /**
   * @brief Get the config pointer used for calibrator
   *
   * @return ICalibratorConfig*
   */
  virtual ICalibratorConfig* getConfig();
};

class IInt8EntropyCalibrator : public ICalibrator {
 public:
  virtual ~IInt8EntropyCalibrator() noexcept = default;
  CalibrationAlgoType getAlgorithm() noexcept override {
    return CalibrationAlgoType::KL_ENTROPY;
  }
};

class IInt8MaxMinCalibrator : public ICalibrator {
 public:
  virtual ~IInt8MaxMinCalibrator() noexcept = default;
  CalibrationAlgoType getAlgorithm() noexcept override {
    return CalibrationAlgoType::MAX_MIN;
  }
};

class IInt8MaxMinEMACalibrator : public ICalibrator {
 public:
  virtual ~IInt8MaxMinEMACalibrator() = default;
  CalibrationAlgoType getAlgorithm() noexcept override {
    return CalibrationAlgoType::MAX_MIN_EMA;
  }
};

class IInt8PercentCalibrator : public ICalibrator {
 public:
  virtual ~IInt8PercentCalibrator() = default;
  CalibrationAlgoType getAlgorithm() noexcept override {
    return CalibrationAlgoType::PERCENTILE;
  }
};

// doxygen end ICalibrator
/**
 * @}
 */

/**
 *  @defgroup init_and_finish topsInference init and finish functions
 *  @{
 *  This section describes the TopsInference init and finish functions.
 *
 */

/**
 *  @defgroup TopsInference_init
 *  @{
 *  This section describes the TopsInference TopsInference_init function.
 *
 */

/**
 * @brief sdk init function
 * only call once at the beginning of the process
 * @return TIFStatus
 */
extern "C" TOPS_INFERENCE_EXPORT TIFStatus topsInference_init();

// doxygen end TopsInference_init
/**
 * @}
 */

/**
 *  @defgroup TopsInference_finish
 *  @{
 *  This section describes the TopsInference TopsInference_finish function.
 *
 */

/**
 * @brief sdk finish function
 * only call once at the end of the process
 * @return TIFStatus
 */
extern "C" TOPS_INFERENCE_EXPORT TIFStatus topsInference_finish();

// doxygen end TopsInference_finish
/**
 * @}
 */

// doxygen end init_and_finish
/**
 * @}
 */

/**
 *  @defgroup device device functions
 *  @{
 *  This section describes the TopsInference device functions.
 *
 */

typedef void* handler_t;

/**
 *  @defgroup Set_device
 *  @{
 *  This section describes the TopsInference set_device function.
 *
 */

/**
 * @brief specify the running device until release. set_device are isolated
 * from each other under multi-process.
 * 1. Under multi-thread condition, each sub-thread will exclusively utilize
 * the claimed resource if set_device is called within the sub-thread.
 * 2. if set_device() is called in main thread, not called in sub thread,
 * the sub thread will share the cluster resources claimed by main thread.
 * 3. if both main thread and sub thread claimed resource with set_device(),
 * the resource claimed by sub thread is used within sub thread.
 * 4. if some sub threads claims resource with set_device(), some does not,
 * each sub thread individually follow the above rule 3 and 2 based on its
 * resource claiming status.
 * @param card_id Specify the current card id
 * @param cluster_ids Specify the current clusters ids
 * @param cluster_ids_size Specify the current cluster num
 * @return handler_t Specify the handler
 */
extern "C" TOPS_INFERENCE_EXPORT handler_t set_device(
    uint32_t card_id, const uint32_t* cluster_ids,
    uint32_t cluster_ids_size = 1, IErrorManager* error_manager = nullptr);

// doxygen end Set_device
/**
 * @}
 */

/**
 *  @defgroup Release_device
 *  @{
 *  This section describes the TopsInference release_device function.
 *
 */

/**
 * @brief free the running device
 * @param handler_t use set_device() handler_t
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_device(handler_t);

// doxygen end Release_device
/**
 * @}
 */

// doxygen end device
/**
 * @}
 */

/**
 *  @defgroup error_manager error_manager functions
 *  @{
 *  This section describes the TopsInference error_manager functions.
 *
 */

/**
 *  @defgroup Create_error_manager
 *  @{
 *  This section describes the TopsInference create_error_manager function.
 *
 */

/**
 * @brief Create an instance of IErrorManager
 *
 */
extern "C" TOPS_INFERENCE_EXPORT IErrorManager* create_error_manager();

// doxygen end Create_error_manager
/**
 * @}
 */

/**
 *  @defgroup Release_error_manager
 *  @{
 *  This section describes the TopsInference release_error_manager function.
 *
 */

/**
 * @brief Release ErrorManager object
 * @param error_manager pointer of IErrorManager
 *
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_error_manager(
    IErrorManager* error_manager);

// doxygen end Release_error_manager
/**
 * @}
 */

// doxygen end error_manager
/**
 * @}
 */

/**
 *  @defgroup parser parser functions
 *  @{
 *  This section describes the TopsInference parser functions.
 *
 */

/**
 *  @defgroup Create_parser
 *  @{
 *  This section describes the TopsInference create_parser function.
 *
 */

/**
 * @brief Create an instance of IParser with specified parse_type
 * @param parse_type use TIF_ONNX to parse onnx model
 * @return IParser* pointer of IParser, or nullptr if create IParser failure
 * @note Examples:
 * @code
    TopsInference::IParser* parser = TopsInference::create_parser(
        TopsInference::TIF_ONNX);
 * @endcode
 *
 */
extern "C" TOPS_INFERENCE_EXPORT IParser* create_parser(
    ParserType parse_type, IErrorManager* error_manager = nullptr);

// doxygen end Create_parser
/**
 * @}
 */

/**
 *  @defgroup Release_parser
 *  @{
 *  This section describes the TopsInference release_parser function.
 *
 */

/**
 * @brief Release Parser object
 * @param parser pointer of IParser
 *
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_parser(IParser* parser);

// doxygen end Release_parser
/**
 * @}
 */

// doxygen end parser
/**
 * @}
 */

/**
 *  @defgroup optimizer optimizer functions
 *  @{
 *  This section describes the TopsInference optimizer functions.
 *
 */

/**
 *  @defgroup Create_optimizer
 *  @{
 *  This section describes the TopsInference create_optimizer function.
 *
 */

/**
 * @brief Create an instance of IOptimizer
 *
 * @return IOptimizer* pointer of IOptimizer, or nullptr if create IOptimizer
 * failure
 * @note Examples:
 * @code
    TopsInference::IOptimizer* optimizer = TopsInference::create_optimizer();
 * @endcode
 */
extern "C" TOPS_INFERENCE_EXPORT IOptimizer* create_optimizer(
    IErrorManager* error_manager = nullptr);

// doxygen end Create_optimizer
/**
 * @}
 */

/**
 *  @defgroup Release_optimizer
 *  @{
 *  This section describes the TopsInference release_optimizer function.
 *
 */

/**
 * @brief Release Optimizer object
 *
 * @param optimizer pointer of IOptimizer
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_optimizer(IOptimizer* optimizer);

// doxygen end Release_optimizer
/**
 * @}
 */

// doxygen end optimizer
/**
 * @}
 */

/**
 *  @defgroup engine engine functions
 *  @{
 *  This section describes the TopsInference engine functions.
 *
 */

/**
 *  @defgroup Create_engine
 *  @{
 *  This section describes the TopsInference create_engine function.
 *
 */

/**
 * @brief Create an instance of IEngine
 *
 * @return IEngine* pointer of IEngine, or nullptr if create IEngine failure
 * @note Examples:
 * @code
    TopsInference::IEngine* engine = TopsInference::create_engine();
 * @endcode
 */
extern "C" TOPS_INFERENCE_EXPORT IEngine* create_engine(
    IErrorManager* error_manager = nullptr);

// doxygen end Create_engine
/**
 * @}
 */

/**
 *  @defgroup Release_engine
 *  @{
 *  This section describes the TopsInference release_engine function.
 *
 */

/**
 * @brief Release Engine object
 *
 * @param engine pointer of IEngine
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_engine(IEngine* engine);

// doxygen end Release_engine
/**
 * @}
 */

// doxygen end engine
/**
 * @}
 */

/**
 *  @defgroup refit
 *  @{
 *  This section describes the TopsInference refit function.
 *
 */

/**
 *  @defgroup Create_refitter
 *  @{
 *  This section describes the TopsInference create_refitter functions.
 *
 */

/**
 * @brief Refitter Create
 * @param engine pointer of IEngine
 *
 * @return IRefitter* pointer of IRefitter, or nullptr if create IRefitter
 * failture
 */
extern "C" TOPS_INFERENCE_EXPORT IRefitter* create_refitter(
    IEngine* engine, IErrorManager* error_manager = nullptr);

// doxygen end Create_refitter
/**
 * @}
 */

/**
 *  @defgroup Release_refitter
 *  @{
 *  This section describes the TopsInference release_refitter function.
 *
 */

/**
 * @brief Refiter Release
 * @param refitter pointer of IRefitter
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_refitter(IRefitter* refitter);

// doxygen end Release_refitter
/**
 * @}
 */

// doxygen end refit
/**
 * @}
 */

/**
 *  @defgroup network network functions
 *  @{
 *  This section describes the TopsInference network functions.
 *
 */

/**
 *  @defgroup Release_network
 *  @{
 *  This section describes the TopsInference release_network function.
 *
 */

/**
 * @brief Release Network object
 *
 * @param network pointer of INetwork
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool release_network(INetwork* network);

// doxygen end Release_network
/**
 * @}
 */

// doxygen end network
/**
 * @}
 */

/**
 *  @defgroup stream stream functions
 *  @{
 *  This section describes the TopsInference stream functions.
 *
 */

/**
 *  @defgroup Create_stream
 *  @{
 *  This section describes the TopsInference create_stream function.
 *
 */

/**
 * @brief Create an instance of Stream, cannot create more than 1000 streams
 *
 * @param streamCtx the double pointer of stream to be created
 * @return bool whether the stream has been created
 */
extern "C" TOPS_INFERENCE_EXPORT bool create_stream(
    topsInferStream_t* streamCtx);

// doxygen end Create_stream
/**
 * @}
 */

/**
 *  @defgroup Synchronize_stream
 *  @{
 *  This section describes the TopsInference synchronize_stream function.
 *
 */

/**
 * @brief Stream Synchronize, Wait for any pending asynchronous action in
 * stream
 *
 * @param stream the pointer of stream to synchronize
 */
extern "C" TOPS_INFERENCE_EXPORT bool synchronize_stream(
    topsInferStream_t stream);

// doxygen end Synchronize_stream
/**
 * @}
 */

/**
 *  @defgroup Destroy_stream
 *  @{
 *  This section describes the TopsInference destroy_stream function.
 *
 */

/**
 * @brief Destroy stream object
 *
 * @param stream pointer of stream
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool destroy_stream(topsInferStream_t stream);

// doxygen end Destroy_stream
/**
 * @}
 */

// doxygen end stream
/**
 * @}
 */

/**
 *  @defgroup future future functions
 *  @{
 *  This section describes the TopsInference future functions.
 *
 */

/**
 *  @defgroup Create_future
 *  @{
 *  This section describes the TopsInference create_future function.
 *
 */

/**
 * @brief Create an instance of IFuture
 *
 */
extern "C" TOPS_INFERENCE_EXPORT IFuture* create_future();

// doxygen end Create_future
/**
 * @}
 */

/**
 *  @defgroup Destroy_future
 *  @{
 *  This section describes the TopsInference destroy_future function.
 *
 */

/**
 * @brief Destroy Future object
 *
 * @param future pointer of IFuture
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool destroy_future(IFuture* future);

// doxygen end Destroy_future
/**
 * @}
 */

// doxygen end future
/**
 * @}
 */

/**
 *  @defgroup tensor tensor functions
 *  @{
 *  This section describes the TopsInference tensor functions.
 *
 */

/**
 *  @defgroup Create_tensor
 *  @{
 *  This section describes the TopsInference create_tensor function.
 *
 */

/**
 * @brief Create an instance of ITensor
 *
 * @return TensorPtr_t pointer of ITensor, or nullptr if create tensor failture
 */
extern "C" TOPS_INFERENCE_EXPORT TensorPtr_t create_tensor();

// doxygen end Create_tensor
/**
 * @}
 */

/**
 *  @defgroup Destroy_tensor
 *  @{
 *  This section describes the TopsInference destroy_tensor function.
 *
 */

/**
 * @brief Destroy tensor
 *
 * @param itensor pointer of a tensor object
 * @return TIFStatus @see TIFStatus
 */
extern "C" TOPS_INFERENCE_EXPORT TIFStatus destroy_tensor(TensorPtr_t itensor);

// doxygen end Destroy_tensor
/**
 * @}
 */

// doxygen end tensor
/**
 * @}
 */

/**
 *  @defgroup memory memory functions
 *  @{
 *  This section describes the TopsInference memory functions.
 *
 */

/**
 *  @defgroup Mem_alloc
 *  @{
 *  This section describes the TopsInference mem_alloc function.
 *
 */

/**
 * @brief memory allocate
 *
 * @param ptr double pointer of GCU memory to allocate
 * @param size GCU memory size
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool mem_alloc(void** ptr, int64_t size);

// doxygen end Mem_alloc
/**
 * @}
 */

/**
 *  @defgroup Mem_free
 *  @{
 *  This section describes the TopsInference mem_free function.
 *
 */

/**
 * @brief Free GCU device memory
 *
 * @param ptr The allocated buffer address
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool mem_free(void* ptr);

// doxygen end Mem_free
/**
 * @}
 */

/**
 *  @defgroup Mem_copy
 *  @{
 *  This section describes the TopsInference mem_copy function.
 *
 */

/**
 * @brief Copy memory from GCU device memory to Host memory, or copy
 * memory from Host memory to GCU device memory
 * @param src The source buffer address
 * @param dst The destination buffer address
 * @param size The bytes copied from src to dst
 * @param kind The kind of copying, @see MemcpyKind
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool mem_copy(void* src, void* dst,
                                               int64_t size, MemcpyKind kind);

// doxygen end Mem_copy
/**
 * @}
 */

/**
 *  @defgroup Mem_copy_async
 *  @{
 *  This section describes the TopsInference mem_copy_async function.
 *
 */

/**
 * @brief Copy memory from GCU device memory to Host memory async, or
 * copy memory from Host memory to GCU device memory async
 *
 * @param src The source buffer address
 * @param dst The destination buffer address
 * @param size The bytes copied from src to dst
 * @param kind The kind of copying, @see MemcpyKind
 * @param stream The stream on which the mem copy runs,
 * @see topsInferStream_t
 * @return bool
 * @retval true Return true if succeed
 * @retval false Return false if fail
 */
extern "C" TOPS_INFERENCE_EXPORT bool mem_copy_async(void* src, void* dst,
                                                     int64_t size,
                                                     MemcpyKind kind,
                                                     topsInferStream_t stream);

// doxygen end Mem_copy_async
/**
 * @}
 */

// doxygen end memory
/**
 * @}
 */

/**
 *  @defgroup topsinference_version version functions
 *  @{
 *  This section describes the TopsInference topsinference_version functions.
 *
 */

/**
 *  @defgroup version
 *  @{
 *  This section describes the TopsInference version function.
 *
 */

/**
 * @brief TopsInference version information
 * @return VersionInfo, TopsInference version
 *
 */
extern "C" TOPS_INFERENCE_EXPORT VersionInfo version();

// doxygen end version
/**
 * @}
 */

// doxygen end topsinference_version
/**
 * @}
 */

}  // namespace TopsInference

#endif  // __TOPS_INFER_RUNTIME_H__
