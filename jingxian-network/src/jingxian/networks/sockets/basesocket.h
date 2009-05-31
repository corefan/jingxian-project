
#ifndef _base_socket_h_
#define _base_socket_h_

#include "jingxian/config.h"

#if !defined (JINGXIAN_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JINGXIAN_LACKS_PRAGMA_ONCE */

// Include files
# include "Winsock2.h"
# include "Mswsock.h"
# include "jingxian/networks/inet_address.h"

typedef WSABUF iovec;

# ifndef ___iopack___
# define ___iopack___
 typedef TRANSMIT_PACKETS_ELEMENT iopack;
# endif // ___iopack___

 # ifndef ___iofile___
# define ___iopack___
 typedef TRANSMIT_FILE_BUFFERS io_file_buf;
# endif // ___iopack___


_jingxian_begin

enum select_mode
{
	  select_read = 1
	, select_write = 2
	, select_error = 4
};

class BaseSocket
{
public:

  ~BaseSocket (void);

  /**
   * �Ƿ񴴽���Ч
   */
  bool is_good() const;

  /**
   * ����˫�����ڲ�ֵ
   */
  void swap( BaseSocket& r );

  /**
   * ȡ��socket����
   */
  SOCKET handle (void) const;

  /**
   * ����socket����
   */
  void set_handle (SOCKET handle);

  /**
   * ����socketѡ����setsockopt
   */
  bool set_option (int level,
                  int option,
                  void *optval,
                  int optlen) const;
  /**
   * ���socketѡ����getsockopt
   */
  bool get_option (int level,
                  int option,
                  void *optval,
                  int *optlen) const;

  /**
   * �ر�socket
   */
  void close (void);
  
  /**
   * �жϲ��ȴ�ֱ��socket���Խ��ж�(д)���������������ʱ
   * @params[ in ] timval ��ʱʱ��
   * @params[ in ] mode �жϵĵĲ������ͣ����select_modeö��
   * @return ���Բ�������true
   */
  bool poll( const TIMEVAL& timeval, int select_mode);

  /**
   * ����һ���˿ɶ����ַ���
   */
  const tstring& toString() const;

  /**
   * ��ʼ��socket����
   */
  static bool initializeScket();

  /**
   * �ر�socket����
   */
  static void shutdownSocket();

  BaseSocket (void);

  BaseSocket ( int protocol_family,
			int type,
            int protocol = 0,
            int reuse_addr = 0);

  BaseSocket ( int protocol_family,
			int type,
            int protocol,
            LPWSAPROTOCOL_INFO protocolinfo,
            GROUP g,
             u_long flags,
            int reuse_addr);

  /**
   * ����socket��ѡ��
   * @params[ in ] value ��ȡֵ���ioctlsocket
   */
  bool enable (int value);

  /**
   * ����socket��ѡ��
   * @params[ in ] value ��ȡֵ���ioctlsocket
   */
  bool disable (int value);

  bool open (int protocol_family = AF_INET,
			int type = SOCK_STREAM,
            int protocol = IPPROTO_TCP,
            int reuse_addr = 0 );

  bool open (int protocol_family,
			int type,
            int protocol,
            LPWSAPROTOCOL_INFO protocolinfo,
            GROUP g,
            u_long flags,
            int reuse_addr);

private:
  NOCOPY( BaseSocket );

  SOCKET handle_;
  mutable tstring toString_;

public:
	static LPFN_TRANSMITFILE __transmitfile;
	static LPFN_ACCEPTEX __acceptex;
	static LPFN_TRANSMITPACKETS __transmitpackets;
	static LPFN_CONNECTEX __connectex;
	static LPFN_DISCONNECTEX __disconnectex;
	static LPFN_GETACCEPTEXSOCKADDRS __getacceptexsockaddrs;
};

inline tostream& operator<<( tostream& target, const BaseSocket& s )
{
  if( INVALID_SOCKET == s.handle())
  {
	  target << _T("INVALID_SOCKET" );
  }
  else
  {
	  target << s.handle() ;
  }
  
  return target;
}

#if defined (OS_HAS_INLINED)
#include "jingxian/networks/sockets/BaseSocket.inl"
#endif /* OS_HAS_INLINED */

_jingxian_end

#endif /* _base_socket_h_ */