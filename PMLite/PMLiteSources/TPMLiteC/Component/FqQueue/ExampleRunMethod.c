/*
 *  Entity.c -- implementation of protocol entities of type *Entity*.
 *
 *  This serves as an example of how to write run methods for FCQueues.
 *
 *  A protocol entity of type *Entity* uses two FCQueues, one for Tx and
 *  one for Rx: itsTxQ, and itsRxQ.
 */

#include "FCQueue.h"


/*  Initialization of the *Entity* strucuture.
 */
void EntityInit( Entity* self )
{
	...
	self->itsTxQ = FcqNew();
	self->itsRxQ = FcqNew();
	FcqSetRunMethod( self->itsTxQueue, EntityTxRun );
	FcqSetRunMethod( self->itsTxQueue, EntityRxRun );
	FcqSetContext( self->itsTxQueue, self );
	FcqSetContext( self->itsRxQueue, self );
	...
}

/*  The run method for the Tx queue inside an *Entity*.
 *  This method uses two methods specific to Entity transmission:
 *  EntityDecodeTxMsg() decodes the incoming message.
 *  EntityModifyTxMsg() modifies the incoming message before forwarding.
 */
void EntityTxRun( FCQueue *aQ )
{
	Entity*  theEntity = (Entity*) FcqGetContext( aQ );
	TPacket* theMsg;
	FCQueue* theQ2;

	while ( (theMsg = FcqPeekMsg( aQ )) != 0 )
	{
		if ( (theQ2 = EntityDecodeTxMsg( theEntity, theMsg )) != 0 )
		{
			if ( !FcqCanPut( theQ2 ) )
			{
				FcqWaitOnQ( aQ, theQ2 );
				return;
			}
			FcqGetMsg( aQ );
			EntityModifyMsg( theEntity, theMsg );
			FcqPutMsg( theQ2, theMsg );

		}
	}
}

/*  Interpret the given message and decide if there is an output message.
 *  Return the FCQueue to which to send the output message.
 */
void EntityDecodeTxMsg()
{
    ...
}

/*  Advance the automaton, and modify the given message to produce an output
 *  message.
 */
void EntityModifyMsg( Entity* anEntity, TPacket* aMsg )
{
    ...
}


/*  The run method for the Rx queue inside an *Entity*.
 */
void EntityRxRun( FCQueue *aQ )
{
    ...
}
