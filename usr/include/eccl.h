/*************************************************************************
 * Copyright (c) 2023, ENRIGIN CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#ifndef ECCL_H_
#define ECCL_H_

#include <tops/tops_runtime_api.h>

#define ECCL_MAJOR 3
#define ECCL_MINOR 1
#define ECCL_PATCH 0

#define ECCL_VERSION_CODE 30100
#define ECCL_VERSION(X,Y,Z) ((X) * 10000 + (Y) * 100 + (Z))

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque handle to communicator */
typedef struct ecclComm* ecclComm_t;

#define ECCL_UNIQUE_ID_BYTES 128
typedef struct { char internal[ECCL_UNIQUE_ID_BYTES]; } ecclUniqueId;

/* Error type */
typedef enum { ecclSuccess                 =  0,
               ecclUnhandledTopsError      =  1,
               ecclSystemError             =  2,
               ecclInternalError           =  3,
               ecclInvalidArgument         =  4,
               ecclInvalidUsage            =  5,
               ecclRemoteError             =  6,
               ecclInProgress              =  7,
               ecclNumResults              =  8 } ecclResult_t;


/* Return the ECCL_VERSION_CODE of the ECCL library in the supplied integer.
 * This integer is coded with the MAJOR, MINOR and PATCH level of the
 * ECCL library
 */
ecclResult_t  ecclGetVersion(int *version);
ecclResult_t pecclGetVersion(int *version);

/* Generates an Id to be used in ecclCommInitRank. ecclGetUniqueId should be
 * called once and the Id should be distributed to all ranks in the
 * communicator before calling ecclCommInitRank. */
ecclResult_t  ecclGetUniqueId(ecclUniqueId* uniqueId);
ecclResult_t pecclGetUniqueId(ecclUniqueId* uniqueId);

/* Creates a new communicator (multi thread/process version).
 * rank must be between 0 and nranks-1 and unique within a communicator clique.
 * Each rank is associated to a tops device, which has to be set before calling
 * ecclCommInitRank.
 * ecclCommInitRank implicitly syncronizes with other ranks, so it must be
 * called by different threads/processes or use ecclGroupStart/ecclGroupEnd. */
ecclResult_t  ecclCommInitRank(ecclComm_t* comm, int nranks, ecclUniqueId commId, int rank);
ecclResult_t pecclCommInitRank(ecclComm_t* comm, int nranks, ecclUniqueId commId, int rank);

/* Creates a clique of communicators (single process version).
 * This is a convenience function to create a single-process communicator clique.
 * Returns an array of ndev newly initialized communicators in comms.
 * comms should be pre-allocated with size at least ndev*sizeof(ecclComm_t).
 * If devlist is nullptr, the first ndev TOPS devices are used.
 * Order of devlist defines user-order of processors within the communicator. */
ecclResult_t  ecclCommInitAll(ecclComm_t* comms, int ndev, const int* devlist);
ecclResult_t  pecclCommInitAll(ecclComm_t* comms, int ndev, const int* devlist);

/* Aborts all operations on the communicator and frees resources associated with it. */
ecclResult_t ecclCommAbort(ecclComm_t comm);
ecclResult_t pecclCommAbort(ecclComm_t comm);

/* Returns the error code of the last asynchronous operation on the communicator. */
ecclResult_t  ecclCommGetAsyncError(ecclComm_t comm, ecclResult_t* asyncError);
ecclResult_t pecclCommGetAsyncError(ecclComm_t comm, ecclResult_t* asyncError);

/* Frees resources associated with communicator object, but waits for any operations
 * that might still be running on the device. */
ecclResult_t  ecclCommDestroy(ecclComm_t comm);
ecclResult_t pecclCommDestroy(ecclComm_t comm);

/* Returns a string for each error code. */
const char*  ecclGetErrorString(ecclResult_t result);
const char* pecclGetErrorString(ecclResult_t result);

/* Returns a human-readable message of the last error that occurred. */
const char*  ecclGetLastError(ecclComm_t comm);
const char* peclGetLastError(ecclComm_t comm);

/* Gets the number of ranks in the communicator clique. */
ecclResult_t  ecclCommCount(const ecclComm_t comm, int* count);
ecclResult_t pecclCommCount(const ecclComm_t comm, int* count);

/* Returns the device number associated with the communicator. */
ecclResult_t  ecclCommDevice(const ecclComm_t comm, int* devid);
ecclResult_t pecclCommDevice(const ecclComm_t comm, int* devid);

/* Returns the user-ordered "rank" associated with the communicator. */
ecclResult_t  ecclCommUserRank(const ecclComm_t comm, int* rank);
ecclResult_t pecclCommUserRank(const ecclComm_t comm, int* rank);

/* Reduction operation selector */
typedef enum { ecclNumOps_dummy = 5 } ecclRedOp_dummy_t;
typedef enum { ecclSum        = 0,
               ecclProd       = 1,
               ecclMax        = 2,
               ecclMin        = 3,
               ecclAvg        = 4,
               /* ecclNumOps: The number of built-in ecclRedOp_t values. Also
                * serves as the least possible value for dynamic ecclRedOp_t's
                * as constructed by ecclRedOpCreate*** functions. */
               ecclNumOps     = 5,
               /* ecclMaxRedOp: The largest valid value for ecclRedOp_t.
                * It is defined to be the largest signed value (since compilers
                * are permitted to use signed enums) that won't grow
                * sizeof(ecclRedOp_t) when compared to previous eccl versions to
                * maintain ABI compatibility. */
               ecclMaxRedOp   = 0x7fffffff>>(32-8*sizeof(ecclRedOp_dummy_t)) } ecclRedOp_t;

// Data types supported by eccl
// Note: only ecclBroadcast and ecclAllGather support 64 bit data types now.
typedef enum { ecclInt8       = 0, ecclChar       = 0,
               ecclUint8      = 1,
               ecclInt32      = 2, ecclInt        = 2,
               ecclUint32     = 3,
               ecclInt64      = 4,
               ecclUint64     = 5,
               ecclFloat16    = 6, ecclHalf       = 6,
               ecclFloat32    = 7, ecclFloat      = 7,
               ecclFloat64    = 8, ecclDouble     = 8,
               ecclBfloat16   = 9,
               ecclNumTypes   = 10 } ecclDataType_t;

/* ecclScalarResidence_t: Location and dereferencing logic for scalar arguments. */
typedef enum {
  /* ecclScalarDevice: The scalar is in device-visible memory and will be
   * dereferenced while the collective is running. */
  ecclScalarDevice = 0,

  /* ecclScalarHostImmediate: The scalar is in host-visible memory and will be
   * dereferenced before the ecclRedOpCreate***() function returns. */
  ecclScalarHostImmediate = 1
} ecclScalarResidence_t;

/*
 * ecclRedOpCreatePreMulSum
 *
 * Creates a new reduction operator which pre-multiplies input values by a given
 * scalar locally before reducing them with peer values via summation. For use
 * only with collectives launched against *comm* and *datatype*. The
 * *residence* argument indicates how/when the memory pointed to by *scalar*
 * will be dereferenced. Upon return, the newly created operator's handle
 * is stored in *op*.
 */
ecclResult_t  ecclRedOpCreatePreMulSum(ecclRedOp_t *op, void *scalar, ecclDataType_t datatype, ecclScalarResidence_t residence, ecclComm_t comm);
ecclResult_t pecclRedOpCreatePreMulSum(ecclRedOp_t *op, void *scalar, ecclDataType_t datatype, ecclScalarResidence_t residence, ecclComm_t comm);

/*
 * ecclRedOpDestroy
 *
 * Destroys the reduction operator *op*. The operator must have been created by
 * ecclRedOpCreatePreMul with the matching communicator *comm*. An operator may be
 * destroyed as soon as the last eccl function which is given that operator returns.
 */
ecclResult_t  ecclRedOpDestroy(ecclRedOp_t op, ecclComm_t comm);
ecclResult_t pecclRedOpDestroy(ecclRedOp_t op, ecclComm_t comm);

/*
 * Collective communication operations
 *
 * Collective communication operations must be called separately for each
 * communicator in a communicator clique.
 *
 * They return when operations have been enqueued on the tops stream.
 */


/*
 * Broadcast
 *
 * Copies count values from root to all other devices.
 * root is the rank (not the tops device) where data resides before the
 * operation is started.
 *
 * In-place operation will happen if sendbuff == recvbuff.
 */
ecclResult_t  ecclBroadcast(const void* sendbuff, void* recvbuff, size_t count, ecclDataType_t datatype, int root,
    ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclBroadcast(const void* sendbuff, void* recvbuff, size_t count, ecclDataType_t datatype, int root,
    ecclComm_t comm, topsStream_t stream);

/*
 * Reduce
 *
 * Reduces data arrays of length count in sendbuff into recvbuff using op
 * operation.
 * recvbuff may be NULL on all calls except for root device.
 * root is the rank (not the tops device) where data will reside after the
 * operation is complete.
 *
 * In-place operation will happen if sendbuff == recvbuff.
 */
ecclResult_t  ecclReduce(const void* sendbuff, void* recvbuff, size_t count, ecclDataType_t datatype,
    ecclRedOp_t op, int root, ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclReduce(const void* sendbuff, void* recvbuff, size_t count, ecclDataType_t datatype,
    ecclRedOp_t op, int root, ecclComm_t comm, topsStream_t stream);

/*
 * All-Reduce
 *
 * Reduces data arrays of length count in sendbuff using op operation, and
 * leaves identical copies of result on each recvbuff.
 *
 * In-place operation will happen if sendbuff == recvbuff.
 */
ecclResult_t  ecclAllReduce(const void* sendbuff, void* recvbuff, size_t count,
    ecclDataType_t datatype, ecclRedOp_t op, ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclAllReduce(const void* sendbuff, void* recvbuff, size_t count,
    ecclDataType_t datatype, ecclRedOp_t op, ecclComm_t comm, topsStream_t stream);

/*
 * Reduce-Scatter
 *
 * Reduces data in sendbuff using op operation and leaves reduced result
 * scattered over the devices so that recvbuff on rank i will contain the i-th
 * block of the result.
 * Assumes sendcount is equal to nranks*recvcount, which means that sendbuff
 * should have a size of at least nranks*recvcount elements.
 *
 * In-place operations will happen if recvbuff == sendbuff + rank * recvcount.
 */
ecclResult_t  ecclReduceScatter(const void* sendbuff, void* recvbuff,
    size_t recvcount, ecclDataType_t datatype, ecclRedOp_t op, ecclComm_t comm,
    topsStream_t stream);
ecclResult_t pecclReduceScatter(const void* sendbuff, void* recvbuff,
    size_t recvcount, ecclDataType_t datatype, ecclRedOp_t op, ecclComm_t comm,
    topsStream_t stream);

/*
 * All-Gather
 *
 * Each device gathers sendcount values from other GCUs into recvbuff,
 * receiving data from rank i at offset i*sendcount.
 * Assumes recvcount is equal to nranks*sendcount, which means that recvbuff
 * should have a size of at least nranks*sendcount elements.
 *
 * In-place operations will happen if sendbuff == recvbuff + rank * sendcount.
 */
ecclResult_t  ecclAllGather(const void* sendbuff, void* recvbuff, size_t sendcount,
    ecclDataType_t datatype, ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclAllGather(const void* sendbuff, void* recvbuff, size_t sendcount,
    ecclDataType_t datatype, ecclComm_t comm, topsStream_t stream);

/*
 * Gather
 *
 * other GCU device(rank i) will send values(data block offset is i*sendcount, size is sendcount)
 * to root GCU device's <recvbuff>
 *
 * If root device wants to recieve a size of nranks*sendcount elements, recvcount should be equal to nranks*sendcount
 *
 * If (recvbuff + rank * sendcount) is equal to sendbuff, In-place operation scenario will trigger.
 */
ecclResult_t  ecclGather(const void* sendbuff, void* recvbuff, size_t sendcount,
    ecclDataType_t datatype, int root, ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclGather(const void* sendbuff, void* recvbuff, size_t sendcount,
    ecclDataType_t datatype, int root, ecclComm_t comm, topsStream_t stream);

/*
 * Scatter
 *
 * Root device will send i-th block of data to other device(rank i), so that the rank i device will recieve this block
 *
 * If root device wants to send a size of nranks*recvcount elements, sendcount should be equal to nranks*recvcount
 *
 * If (sendbuff + rank * recvcount) is equal to recvbuff, In-place operation scenario will trigger.
 */
ecclResult_t  ecclScatter(const void* sendbuff, void* recvbuff,
                          size_t recvcount, ecclDataType_t datatype, int root,
                          ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclScatter(const void* sendbuff, void* recvbuff,
                          size_t recvcount, ecclDataType_t datatype, int root,
                          ecclComm_t comm, topsStream_t stream);

/*
 * Send
 *
 * Send data from sendbuff to rank peer.
 * Rank peer needs to call ecclRecv with the same datatype and the same count from this
 * rank.
 */
ecclResult_t  ecclSend(const void* sendbuff, size_t count,
                       ecclDataType_t datatype, int peer, ecclComm_t comm,
                       topsStream_t stream);
ecclResult_t pecclSend(const void* sendbuff, size_t count,
                       ecclDataType_t datatype, int peer, ecclComm_t comm,
                       topsStream_t stream);

/*
 * Receive
 *
 * Receive data from rank peer into recvbuff.
 * Rank peer needs to call ecclSend with the same datatype and the same count to this
 * rank.
 */
ecclResult_t  ecclRecv(void* recvbuff, size_t count, ecclDataType_t datatype,
                       int peer, ecclComm_t comm, topsStream_t stream);
ecclResult_t pecclRecv(void* recvbuff, size_t count, ecclDataType_t datatype,
                       int peer, ecclComm_t comm, topsStream_t stream);

/*
 * Group Start
 *
 * Start a group call. All calls to ECCL until ecclGroupEnd will be fused into
 * a single ECCL operation. Nothing will be started on the stream until
 * ecclGroupEnd.
 * Note: only support same eccl operation aggregation now.
 */
ecclResult_t  ecclGroupStart();
ecclResult_t pecclGroupStart();

/*
 * Group End
 *
 * End a group call. Start a fused ECCL operation consisting of all calls since
 * ecclGroupStart. Operations on the TOPS stream depending on the ECCL operations
 * need to be called after ecclGroupEnd.
 */
ecclResult_t  ecclGroupEnd();
ecclResult_t pecclGroupEnd();

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // end include guard
