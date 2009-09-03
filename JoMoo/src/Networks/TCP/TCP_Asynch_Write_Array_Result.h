

#ifndef JOMOO_TCP_ASYNCH_WRITE_ARRAY_RESULT_H
#define JOMOO_TCP_ASYNCH_WRITE_ARRAY_RESULT_H

#include "config.h"

#if !defined (JOMOO_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JOMOO_LACKS_PRAGMA_ONCE */

// Include files
#include "../config_Networks.h"
#include "TCP_Asynch_Result.H"

_networks_begin

class TCP_Write_Array_Result : public TCP_Asynch_Result
{
public:
	typedef TCP_Transport_Handler handler_type;
	TCP_Write_Array_Result( );

	void init( Instance_TCP* instance
		, TcpTransport* channel
		, handler_type* handle
		, const iovec* buffer
		, size_t length
		, void * act );

	void fini();

	/**
	* ȡ�����ݿ�Ļ���
	* @return ���ݻ����ָ��
	*/
	const iovec* data();

	/**
	* ȡ�����ݻ���Ĵ�С
	* @return ���ݻ���Ĵ�С
	*/
	size_t size() const;

	/**
	* �����첽�����Ļص����
	*/
	handler_type& handle();

	/**
	* �����첽������channel���
	*/
	TcpTransport& native_transport();

	JOMOO_Transport& transport();


	void complete (size_t bytes_transferred,
		int success,
		const void *completion_key,
		u_long error = 0);

private:

	TcpTransport* m_channel_;

	handler_type* m_handle_;

	const iovec* m_data_;

	size_t m_size_;

};
_networks_end

#endif // JOMOO_TCP_ASYNCH_WRITE_ARRAY_RESULT_H