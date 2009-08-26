
# include "pro_config.h"
# include "jingxian/networks/OutgoingBuffer.h"
# include "jingxian/networks/commands/WriteCommand.H"
# include "jingxian/networks/ConnectedSocket.h"

_jingxian_begin

OutgoingBuffer::OutgoingBuffer()
: connectedSocket_(null_ptr)
{
}

OutgoingBuffer::~OutgoingBuffer()
{
}

void OutgoingBuffer::initialize(ConnectedSocket* connectedSocket)
{
	connectedSocket_ = connectedSocket;
}

void OutgoingBuffer::send(buffer_chain_t* buf)
{
	assertBuffer(buf);
	buffer_.push(buf);
}

ICommand* OutgoingBuffer::makeCommand()
{
	buffer_chain_t* current = buffer_.next(null_ptr);
	if(is_null(current))
		return null_ptr;

	switch( current->type)
	{
	case BUFFER_ELEMENT_MEMORY:
		{
			std::auto_ptr<WriteCommand> command(new WriteCommand(connectedSocket_));
			io_mem_buf iobuf;
			do
			{
				databuffer_t* data = databuffer_cast(current);

				iobuf.buf = data->start;
				iobuf.len = (int)(data->end - data->start);
				assert(0 <= iobuf.len);
				if(0 < iobuf.len)
					command->iovec().push_back(iobuf);
			}
			while(null_ptr != (current = buffer_.next(current))
				&& BUFFER_ELEMENT_MEMORY == current->type);

			if(command->iovec().empty())
				return null_ptr;

			return command.release();
		}
	case BUFFER_ELEMENT_FILE:
		{
			// TODO: ������ļ���֧��
		filebuffer_t* filebuf = filebuffer_cast(current);
		assert( false );
		return null_ptr;
		}
	case BUFFER_ELEMENT_PACKET:
		{
			// TODO: ������ļ���֧��
		packetbuffer_t* packetbuf = packetbuffer_cast(current);
		assert( false );
		return null_ptr;
		}
	default:
		assert( false );
		return null_ptr;
	}
}

bool OutgoingBuffer::clearBytes(size_t len)
{
	size_t exceptLen = len;
	buffer_chain_t* current = null_ptr;
	while(null_ptr != (current = buffer_.head()))
	{	
		switch( current->type)
		{
		case BUFFER_ELEMENT_MEMORY:
			{
			databuffer_t* data = databuffer_cast(current);
			size_t dataLen = data->end - data->start;
			if( dataLen >= exceptLen)
			{
				data->start += exceptLen;
				if( dataLen == exceptLen)
					freebuffer(buffer_.pop());

				exceptLen = 0;
				return false;
			}

			data->start += dataLen;
			exceptLen -= dataLen;
			break;
			}
		case BUFFER_ELEMENT_FILE:
			{
				// TODO: ������ļ���֧��
			filebuffer_t* filebuf = filebuffer_cast(current);
			assert( false );
			return false;
			}
		case BUFFER_ELEMENT_PACKET:
			{
				// TODO: ������ļ���֧��
			packetbuffer_t* packetbuf = packetbuffer_cast(current);
			assert( false );
			return false;
			}
		default:
			assert( false );
			return false;
		}

		freebuffer(buffer_.pop());
	}

	return (0 == exceptLen);
}


void assertBuffer(buffer_chain_t* newbuf)
{
	switch( newbuf->type)
	{
	case BUFFER_ELEMENT_MEMORY:
		{
			databuffer_t* data = databuffer_cast(newbuf);
			assert( data->ptr <= data->start );
			assert( data->start <= data->end);
			assert( data->end <= data->ptr + data->capacity);
			break;
		}
	case BUFFER_ELEMENT_FILE:
		{
			// TODO: ������ļ���֧��
			filebuffer_t* filebuf = filebuffer_cast(newbuf);
			assert( false );
			break;
		}
	case BUFFER_ELEMENT_PACKET:
		{
			// TODO: ������ļ���֧��
			packetbuffer_t* packetbuf = packetbuffer_cast(newbuf);
			assert( false );
			break;
		}
	default:
		assert( false );
		break;
	}
}

_jingxian_end