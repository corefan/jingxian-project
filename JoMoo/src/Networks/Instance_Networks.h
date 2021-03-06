

#ifndef Instance_Networks_H
#define Instance_Networks_H

#include "config.h"

#if !defined (JOMOO_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* JOMOO_LACKS_PRAGMA_ONCE */

// Include files
#include "config_Networks.h"
#include "WIN32OperationProactor.H"
#include "Base/Util/Properties.H"
#include "include/Endpoint_Factory.H"
#include "include/Communicator.H"

_networks_begin

class InstanceNetworks : public Instance_Networks
{
public:

	typedef NetWorksLoggerPtr LoggerPtr;

	InstanceNetworks(void);
	
	~InstanceNetworks(void);

	int open( _util ConfigurationSettings& config );

	void close( );

	bool isStop();

	proactor& getNativeProactor();

	JOMOO_Operation_Proactor& getProactor();

	int run( u_long milli_seconds );

	LoggerPtr getLogger();

	void setLogger( LoggerPtr );

	const _util ConfigurationSettings& conf() const;

	JOMOO_Endpoint_Ptr create(const tstring&);

	_micro_kernel Component& findComponent( const JOMOO_Url_Base& url );

	_micro_kernel IKernel& kernel( );

	const tstring& toString() const;

private:

	DECLARE_NO_COPY_CLASS( InstanceNetworks );

	proactor m_proactor_;

	//std::auto_ptr< EndpointFactoryManager > EndpointManager_;

	LoggerPtr Logger_;

	_util ConfigurationSettings config_;

	tstring toString_;
};

_networks_end

#endif // Instance_Networks_H