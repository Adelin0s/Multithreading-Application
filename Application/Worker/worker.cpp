#include "Worker/worker.h"

namespace Eset
{
	Worker::Worker( const uint32 numberOfWorkers ) :
		m_numberOfWorkers( numberOfWorkers )
	{
		for( uint32 i = 0; i < m_numberOfWorkers; ++i )
		{
			m_threadPool.emplace_back( std::jthread( &Worker::WorkerRoutine, this ) );
		}
	}

	void Worker::Push( Task &&task )
	{
		std::unique_lock lock( m_lock );
		m_tasks.push( std::forward< Task >( task ) );
		m_condition.notify_one( );
	}

	void Worker::Done( )
	{
		std::unique_lock lock( m_lock );
		m_done = true;
		m_condition.notify_all( );
	}

	void Worker::WorkerRoutine( )
	{
		Task func;
		while( true )
		{
			{
				std::unique_lock lock( m_lock );
				m_condition.wait( lock, [ this ]
								  { return !m_tasks.empty( ) || m_done; } );
				if( m_done && m_tasks.empty( ) )
				{
					return;
				}
				func = m_tasks.front( );
				m_tasks.pop( );
			}
			func( );
		}
	}
} // namespace Eset
