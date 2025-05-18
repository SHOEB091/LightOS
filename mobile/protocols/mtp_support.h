/**
 * LightOS Mobile
 * MTP Protocol Support header
 */

#ifndef MTP_SUPPORT_H
#define MTP_SUPPORT_H

#include "../mobile_manager.h"

// MTP operation codes
#define MTP_OPERATION_GET_DEVICE_INFO 0x1001
#define MTP_OPERATION_OPEN_SESSION 0x1002
#define MTP_OPERATION_CLOSE_SESSION 0x1003
#define MTP_OPERATION_GET_STORAGE_IDS 0x1004
#define MTP_OPERATION_GET_STORAGE_INFO 0x1005
#define MTP_OPERATION_GET_NUM_OBJECTS 0x1006
#define MTP_OPERATION_GET_OBJECT_HANDLES 0x1007
#define MTP_OPERATION_GET_OBJECT_INFO 0x1008
#define MTP_OPERATION_GET_OBJECT 0x1009
#define MTP_OPERATION_GET_THUMB 0x100A
#define MTP_OPERATION_DELETE_OBJECT 0x100B
#define MTP_OPERATION_SEND_OBJECT_INFO 0x100C
#define MTP_OPERATION_SEND_OBJECT 0x100D
#define MTP_OPERATION_FORMAT_STORAGE 0x100F
#define MTP_OPERATION_RESET_DEVICE 0x1010
#define MTP_OPERATION_GET_DEVICE_PROP_DESC 0x1014
#define MTP_OPERATION_GET_DEVICE_PROP_VALUE 0x1015
#define MTP_OPERATION_SET_DEVICE_PROP_VALUE 0x1016
#define MTP_OPERATION_RESET_DEVICE_PROP_VALUE 0x1017
#define MTP_OPERATION_GET_OBJECT_PROPS_SUPPORTED 0x9801
#define MTP_OPERATION_GET_OBJECT_PROP_DESC 0x9802
#define MTP_OPERATION_GET_OBJECT_PROP_VALUE 0x9803
#define MTP_OPERATION_SET_OBJECT_PROP_VALUE 0x9804
#define MTP_OPERATION_GET_OBJECT_PROP_LIST 0x9805
#define MTP_OPERATION_SET_OBJECT_PROP_LIST 0x9806
#define MTP_OPERATION_GET_OBJECT_REFERENCES 0x9810
#define MTP_OPERATION_SET_OBJECT_REFERENCES 0x9811
#define MTP_OPERATION_COPY_OBJECT 0x9817
#define MTP_OPERATION_MOVE_OBJECT 0x9818
#define MTP_OPERATION_GET_PARTIAL_OBJECT 0x9804
#define MTP_OPERATION_GET_PARTIAL_OBJECT_64 0x95C1
#define MTP_OPERATION_SEND_PARTIAL_OBJECT 0x9805
#define MTP_OPERATION_GET_OBJECT_PROPS 0x9803
#define MTP_OPERATION_SET_OBJECT_PROPS 0x9804

// MTP response codes
#define MTP_RESPONSE_OK 0x2001
#define MTP_RESPONSE_GENERAL_ERROR 0x2002
#define MTP_RESPONSE_SESSION_NOT_OPEN 0x2003
#define MTP_RESPONSE_INVALID_TRANSACTION_ID 0x2004
#define MTP_RESPONSE_OPERATION_NOT_SUPPORTED 0x2005
#define MTP_RESPONSE_PARAMETER_NOT_SUPPORTED 0x2006
#define MTP_RESPONSE_INCOMPLETE_TRANSFER 0x2007
#define MTP_RESPONSE_INVALID_STORAGE_ID 0x2008
#define MTP_RESPONSE_INVALID_OBJECT_HANDLE 0x2009
#define MTP_RESPONSE_DEVICE_PROP_NOT_SUPPORTED 0x200A
#define MTP_RESPONSE_INVALID_OBJECT_FORMAT_CODE 0x200B
#define MTP_RESPONSE_STORE_FULL 0x200C
#define MTP_RESPONSE_OBJECT_WRITE_PROTECTED 0x200D
#define MTP_RESPONSE_STORE_READ_ONLY 0x200E
#define MTP_RESPONSE_ACCESS_DENIED 0x200F
#define MTP_RESPONSE_NO_THUMBNAIL_PRESENT 0x2010
#define MTP_RESPONSE_SELF_TEST_FAILED 0x2011
#define MTP_RESPONSE_PARTIAL_DELETION 0x2012
#define MTP_RESPONSE_STORE_NOT_AVAILABLE 0x2013
#define MTP_RESPONSE_SPECIFICATION_BY_FORMAT_UNSUPPORTED 0x2014
#define MTP_RESPONSE_NO_VALID_OBJECT_INFO 0x2015
#define MTP_RESPONSE_DEVICE_BUSY 0x2019
#define MTP_RESPONSE_INVALID_PARENT_OBJECT 0x201A
#define MTP_RESPONSE_INVALID_DEVICE_PROP_FORMAT 0x201B
#define MTP_RESPONSE_INVALID_DEVICE_PROP_VALUE 0x201C
#define MTP_RESPONSE_INVALID_PARAMETER 0x201D
#define MTP_RESPONSE_SESSION_ALREADY_OPEN 0x201E
#define MTP_RESPONSE_TRANSACTION_CANCELLED 0x201F
#define MTP_RESPONSE_SPECIFICATION_OF_DESTINATION_UNSUPPORTED 0x2020
#define MTP_RESPONSE_INVALID_OBJECT_PROP_CODE 0xA801
#define MTP_RESPONSE_INVALID_OBJECT_PROP_FORMAT 0xA802
#define MTP_RESPONSE_INVALID_OBJECT_PROP_VALUE 0xA803
#define MTP_RESPONSE_INVALID_OBJECT_REFERENCE 0xA804
#define MTP_RESPONSE_GROUP_NOT_SUPPORTED 0xA805
#define MTP_RESPONSE_INVALID_DATASET 0xA806
#define MTP_RESPONSE_SPECIFICATION_BY_GROUP_UNSUPPORTED 0xA807
#define MTP_RESPONSE_SPECIFICATION_BY_DEPTH_UNSUPPORTED 0xA808
#define MTP_RESPONSE_OBJECT_TOO_LARGE 0xA809
#define MTP_RESPONSE_OBJECT_PROP_NOT_SUPPORTED 0xA80A

// MTP object format codes
#define MTP_FORMAT_UNDEFINED 0x3000
#define MTP_FORMAT_ASSOCIATION 0x3001
#define MTP_FORMAT_SCRIPT 0x3002
#define MTP_FORMAT_EXECUTABLE 0x3003
#define MTP_FORMAT_TEXT 0x3004
#define MTP_FORMAT_HTML 0x3005
#define MTP_FORMAT_DPOF 0x3006
#define MTP_FORMAT_AIFF 0x3007
#define MTP_FORMAT_WAV 0x3008
#define MTP_FORMAT_MP3 0x3009
#define MTP_FORMAT_AVI 0x300A
#define MTP_FORMAT_MPEG 0x300B
#define MTP_FORMAT_ASF 0x300C
#define MTP_FORMAT_JPEG 0x3801
#define MTP_FORMAT_TIFF 0x3802
#define MTP_FORMAT_BMP 0x3804
#define MTP_FORMAT_GIF 0x3807
#define MTP_FORMAT_JFIF 0x3808
#define MTP_FORMAT_PNG 0x380B
#define MTP_FORMAT_TIFF_EP 0x380D
#define MTP_FORMAT_JP2 0x380F
#define MTP_FORMAT_JPX 0x3810

// MTP functions
int mtp_init();
int mtp_detect_devices(mobile_device_t** devices, unsigned int* count);
int mtp_connect(mobile_device_t* device);
int mtp_disconnect(mobile_device_t* device);
int mtp_get_device_info(mobile_device_t* device, char* info, unsigned int info_size);
int mtp_get_storage_ids(mobile_device_t* device, unsigned int** storage_ids, unsigned int* count);
int mtp_get_storage_info(mobile_device_t* device, unsigned int storage_id, char* info, unsigned int info_size);
int mtp_get_object_handles(mobile_device_t* device, unsigned int storage_id, unsigned int format, unsigned int parent, unsigned int** handles, unsigned int* count);
int mtp_get_object_info(mobile_device_t* device, unsigned int handle, char* info, unsigned int info_size);
int mtp_get_object(mobile_device_t* device, unsigned int handle, const char* local_path);
int mtp_get_thumb(mobile_device_t* device, unsigned int handle, const char* local_path);
int mtp_delete_object(mobile_device_t* device, unsigned int handle);
int mtp_send_object_info(mobile_device_t* device, unsigned int storage_id, unsigned int parent, const char* name, unsigned int format, unsigned int* handle);
int mtp_send_object(mobile_device_t* device, unsigned int handle, const char* local_path);
int mtp_format_storage(mobile_device_t* device, unsigned int storage_id);
int mtp_reset_device(mobile_device_t* device);
int mtp_get_device_prop_desc(mobile_device_t* device, unsigned int prop_code, char* desc, unsigned int desc_size);
int mtp_get_device_prop_value(mobile_device_t* device, unsigned int prop_code, char* value, unsigned int value_size);
int mtp_set_device_prop_value(mobile_device_t* device, unsigned int prop_code, const char* value);
int mtp_reset_device_prop_value(mobile_device_t* device, unsigned int prop_code);
int mtp_get_object_props_supported(mobile_device_t* device, unsigned int format, unsigned int** prop_codes, unsigned int* count);
int mtp_get_object_prop_desc(mobile_device_t* device, unsigned int prop_code, unsigned int format, char* desc, unsigned int desc_size);
int mtp_get_object_prop_value(mobile_device_t* device, unsigned int handle, unsigned int prop_code, char* value, unsigned int value_size);
int mtp_set_object_prop_value(mobile_device_t* device, unsigned int handle, unsigned int prop_code, const char* value);
int mtp_get_object_prop_list(mobile_device_t* device, unsigned int handle, char* prop_list, unsigned int prop_list_size);
int mtp_set_object_prop_list(mobile_device_t* device, unsigned int handle, const char* prop_list);
int mtp_get_object_references(mobile_device_t* device, unsigned int handle, unsigned int** references, unsigned int* count);
int mtp_set_object_references(mobile_device_t* device, unsigned int handle, unsigned int* references, unsigned int count);
int mtp_copy_object(mobile_device_t* device, unsigned int handle, unsigned int storage_id, unsigned int parent);
int mtp_move_object(mobile_device_t* device, unsigned int handle, unsigned int storage_id, unsigned int parent);
int mtp_get_partial_object(mobile_device_t* device, unsigned int handle, unsigned int offset, unsigned int length, const char* local_path);
int mtp_send_partial_object(mobile_device_t* device, unsigned int handle, unsigned int offset, unsigned int length, const char* local_path);

#endif /* MTP_SUPPORT_H */
