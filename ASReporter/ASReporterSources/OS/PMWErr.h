// ===========================================================================
//	PMWErr.h
//	Copyright (C) 1996-97 Smartcode Software, Inc. All rights reserved.
// ===========================================================================

#ifndef PMWErr_h
#define PMWErr_h

//-----------------------------------------------------------------------
#ifndef PMWinInc_h
	#error PMWinInc.h must have been included before this header (PMWErr.h)
#endif

//-----------------------------------------------------------------------
/*
Error mapping given a PMError must provide a WinError AND an Error String
PMErrors are defined using a Class and a Code. 
The first code of a Class must be 1 and other codes must be consecutives or
it will break the algorithm...
An array maps PMError Classes to a base index for this class
The WinError code is this base index plus the Code | bit 29 
(See GetLastError() about bit 29)
The string will be loaded from resources using the WinError Code (&~ bit29)
*/
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
/**	Sockets	*/
/*
const pmuint16 kPMSocketErrorClass = 0x10;

const pmuint32 kPMSocketUnknownError	= 0x100001;
const pmuint32 kPMSocketDNSError		= 0x100002;
const pmuint32 kPMSocketConnectError	= 0x100003;
const pmuint32 kPMSocketReadError		= 0x100004;
const pmuint32 kPMSocketWriteError		= 0x100005;
*/
#define ERROR_PM_SOCKET_UNKNOWN						(0x100L | 0x20000000L)
#define ERROR_PM_SOCKET_HOST_NOT_FOUND				(0x101L | 0x20000000L)
#define ERROR_PM_SOCKET_HOST_UNREACHABLE			(0x102L | 0x20000000L)
#define ERROR_PM_SOCKET_READ						(0x103L | 0x20000000L)
#define ERROR_PM_SOCKET_WRITE						(0x104L | 0x20000000L)

//-----------------------------------------------------------------------
/**	Initialisation	*/
/*
const pmuint16 kPMOSHelperErrorClass = 0x80;

const pmuint32 kPMOSHelperGeneralError = 0x800001;
const pmuint32 kPMOSHelperInitPMError = 0x800002;
const pmuint32 kPMOSHelperInitTCPError = 0x800003;
*/
#define ERROR_PM_INIT_UNKNOWN						(0x800L | 0x20000000L)
#define	ERROR_PM_INIT_PM							(0x801L | 0x20000000L)
#define	ERROR_PM_INIT_TCP							(0x802L | 0x20000000L)

#define ERROR_PM_NOT_INITED							(0x803L | 0x20000000L)
#define ERROR_PM_UNKNOWN							(0x804L | 0x20000000L)
#define ERROR_PM_INVALID_PARAM						(0x805L | 0x20000000L)

#define	ERROR_PM_RESOURCE							(0x806L | 0x20000000L)
#define ERROR_PM_USER_ABORTED						(0x807L | 0x20000000L)

//-----------------------------------------------------------------------
/**	Internet protocol	*/
/*
const pmuint16 kPMInetErrorClass = 0x100;

const pmuint32 kPMInetGeneralError = 0x1000001;
const pmuint32 kPMInetDNSError = 0x1000002;
const pmuint32 kPMInetConnectError = 0x1000003;
const pmuint32 kPMInetSendError = 0x1000004;
const pmuint32 kPMInetReceiveError = 0x1000005;
const pmuint32 kPMInetBusyError = 0x1000006;
const pmuint32 kPMInetConnectionAbortedError = 0x1000007;
const pmuint32 kPMInetNotLoggedError = 0x1000008;
const pmuint32 kPMInetCommandRefusedError = 0x1000009;
const pmuint32 kPMInetUserAbortedError = 0x100000a;
*/
#define ERROR_PM_INET_UNKNOWN						(0x1000L | 0x20000000L)
#define ERROR_PM_INET_HOST_NOT_FOUND				(0x1001L | 0x20000000L)
#define ERROR_PM_INET_HOST_UNREACHABLE				(0x1002L | 0x20000000L)
#define ERROR_PM_INET_SEND							(0x1003L | 0x20000000L)
#define ERROR_PM_INET_RECEIVE						(0x1004L | 0x20000000L)
#define ERROR_PM_INET_BUSY							(0x1005L | 0x20000000L)
#define ERROR_PM_INET_CONNECTION_ABORTED			(0x1006L | 0x20000000L)
#define ERROR_PM_INET_NOT_LOGGED					(0x1007L | 0x20000000L)
#define ERROR_PM_INET_COMMAND_REFUSED				(0x1008L | 0x20000000L)
#define ERROR_PM_INET_USER_ABORTED					(0x1009L | 0x20000000L)

//-----------------------------------------------------------------------
/**	POP3	*/
/*
const pmuint16 kPMPOP3ErrorClass = 0x101;

const pmuint32 kPMPOP3GeneralError = 0x1010001;
const pmuint32 kPMPOP3ProtocolError = 0x1010002;
const pmuint32 kPMPOP3AuthenticationNotSupportedError = 0x1010003;
const pmuint32 kPMPOP3MediaReadingError = 0x1010004;
const pmuint32 kPMPOP3MediaFillingError = 0x1010005;
const pmuint32 kPMPOP3SomeMessagesNotDeletedError = 0x1010006;
const pmuint32 kPMPOP3InvalidLoginError = 0x1010007;
*/
#define ERROR_PM_POP3_UNKNOWN						(0x1100L | 0x20000000L)
#define ERROR_PM_POP3_PROTOCOL						(0x1101L | 0x20000000L)
#define ERROR_PM_POP3_AUTHENTICATION_NOT_SUPPORTED	(0x1102L | 0x20000000L)
#define ERROR_PM_POP3_MEDIA_READING					(0x1103L | 0x20000000L)
#define ERROR_PM_POP3_MEDIA_FILLING					(0x1104L | 0x20000000L)
#define ERROR_PM_POP3_SOME_MESSAGES_NOT_DELETED		(0x1105L | 0x20000000L)
#define ERROR_PM_POP3_INVALID_LOGIN					(0x1106L | 0x20000000L)

//-----------------------------------------------------------------------
/**	SMTP	*/
/*
const pmuint16 kPMSMTPErrorClass = 0x102;

const pmuint32 kPMSMTPGeneralError = 0x1020001;
const pmuint32 kPMSMTPProtocolError = 0x1020002;
const pmuint32 kPMSMTPMediaReadingError = 0x1020003;
const pmuint32 kPMSMTPBadFromError = 0x1020004;
const pmuint32 kPMSMTPSomeUnknownRecipientsError = 0x1020005;
const pmuint32 kPMSMTPBadRecipientError = 0x1020006;
*/
#define ERROR_PM_SMTP_UNKNOWN						(0x1200L | 0x20000000L)
#define ERROR_PM_SMTP_PROTOCOL						(0x1201L | 0x20000000L)
#define ERROR_PM_SMTP_MEDIA_READING					(0x1202L | 0x20000000L)
#define ERROR_PM_SMTP_BAD_FROM						(0x1203L | 0x20000000L)
#define ERROR_PM_SMTP_SOME_UNKNOWN_RECIPIENTS		(0x1204L | 0x20000000L)
#define ERROR_PM_SMTP_BAD_RECIPIENT					(0x1205L | 0x20000000L)

//-----------------------------------------------------------------------
/**	FTP	*/
/*
const pmuint16 kPMFTPErrorClass = 0x103;

const pmuint32 kPMFTPGeneralError = 0x1030001;
const pmuint32 kPMFTPProtocolError = 0x1030002;
const pmuint32 kPMFTPAccountNeededError = 0x1030003;
const pmuint32 kPMFTPMediaReadingError = 0x1030004;
const pmuint32 kPMFTPMediaFillingError = 0x1030005;
const pmuint32 kPMFTPDataConnectionError = 0x1030006;
const pmuint32 kPMFTPFileNotFoundError = 0x1030007;
const pmuint32 kPMFTPTransferAbortedError = 0x1030008;
const pmuint32 kPMFTPNoParentError = 0x1030009;
const pmuint32 kPMFTPNotSpecifiedError = 0x103000a;
const pmuint32 kPMFTPNotDirError = 0x103000b;
const pmuint32 kPMFTPNotFileError = 0x103000c;
const pmuint32 kPMFTPInfoNotPresentError = 0x103000d;
*/
#define ERROR_PM_FTP_UNKNOWN						(0x1300L | 0x20000000L)
#define ERROR_PM_FTP_PROTOCOL						(0x1301L | 0x20000000L)
#define ERROR_PM_FTP_ACCOUNT_NEEDED					(0x1302L | 0x20000000L)
#define ERROR_PM_FTP_MEDIA_READING					(0x1303L | 0x20000000L)
#define ERROR_PM_FTP_MEDIA_FILLING					(0x1304L | 0x20000000L)
#define ERROR_PM_FTP_DATA_CONNECTION				(0x1305L | 0x20000000L)
#define ERROR_PM_FTP_FILE_NOT_FOUND					(0x1306L | 0x20000000L)
#define ERROR_PM_FTP_TRANSFER_ABORTED				(0x1307L | 0x20000000L)
#define ERROR_PM_FTP_NO_PARENT						(0x1308L | 0x20000000L)
#define ERROR_PM_FTP_NOT_SPECIFIED					(0x1309L | 0x20000000L)
#define ERROR_PM_FTP_NOT_DIR						(0x130aL | 0x20000000L)
#define ERROR_PM_FTP_NOT_FILE						(0x130bL | 0x20000000L)
#define ERROR_PM_FTP_INFO_NOT_PRESENT				(0x130cL | 0x20000000L)


//-----------------------------------------------------------------------
/**	HTTP	*/
/*
const pmuint16 kPMHTTPErrorClass = 0x105;

const pmuint32 kPMHTTPGeneralError = 0x1050001;
const pmuint32 kPMHTTPBusyError = 0x1050002;
const pmuint32 kPMHTTPUserAbortedError = 0x1050003;

const pmuint32 kPMHTTPBadRequestError = 0x1050004;			// HTTP 400
const pmuint32 kPMHTTPUnauthorizedError = 0x1050005;		// HTTP 401
const pmuint32 kPMHTTPForbiddenError = 0x1050006;			// HTTP 403
const pmuint32 kPMHTTPNotFoundError = 0x1050007;			// HTTP 404
const pmuint32 kPMHTTPServerInternalError = 0x1050008;		// HTTP 500
const pmuint32 kPMHTTPNotImplementedError = 0x1050009;		// HTTP 501
const pmuint32 kPMHTTPBadGatewayError = 0x105000a;			// HTTP 502
const pmuint32 kPMHTTPServiceUnavailableError = 0x105000b;	// HTTP 503
const pmuint32 kPMHTTPUnknownErrorCodeError = 0x105000c;	// HTTP Unknown 4xx or 5xx
*/
#define ERROR_PM_HTTP_UNKNOWN						(0x1500L | 0x20000000L)
#define ERROR_PM_HTTP_BUSY							(0x1501L | 0x20000000L)
#define ERROR_PM_HTTP_USER_ABORTED					(0x1502L | 0x20000000L)
#define ERROR_PM_HTTP_BAD_REQUEST					(0x1503L | 0x20000000L)
#define ERROR_PM_HTTP_UNAUTHORIZED					(0x1504L | 0x20000000L)
#define ERROR_PM_HTTP_FORBIDDEN						(0x1505L | 0x20000000L)
#define ERROR_PM_HTTP_NOT_FOUND						(0x1506L | 0x20000000L)
#define ERROR_PM_HTTP_SERVER_INTERNAL				(0x1507L | 0x20000000L)
#define ERROR_PM_HTTP_NOT_IMPLEMENTED				(0x1508L | 0x20000000L)
#define ERROR_PM_HTTP_BAD_GATEWAY					(0x1509L | 0x20000000L)
#define ERROR_PM_HTTP_SERVICE_UNAVAILABLE			(0x150aL | 0x20000000L)
#define ERROR_PM_HTTP_UNKNOWN_ERROR_CODE			(0x150bL | 0x20000000L)

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
/**	UCP	*/
/**	LowUCP */
/*
const pmuint16 kPMUCPErrorClass = 0x104;

const pmuint32 kPMUCPBadDataError					= 0x1040001;
const pmuint32 kPMUCPTooManyTransactionsError		= 0x1040002;
const pmuint32 kPMUCPBadTransactionNumberError		= 0x1040003;
*/

#define ERROR_PM_UCP_UNKNOWN						(0x1600L | 0x20000000L)
#define ERROR_PM_UCP_BAD_DATA						(0x1601L | 0x20000000L)
#define ERROR_PM_UCP_TOO_MANY_TRANSACTIONS			(0x1602L | 0x20000000L)
#define ERROR_PM_UCP_BAD_TRANSACTION_NUMBER			(0x1603L | 0x20000000L)

/**	UCP	*/
/*
const pmuint32 kPMUCPNoRecipientsError				= 0x1040004;
const pmuint32 kPMUCPIncompatibleMessageError		= 0x1040005;
*/

#define ERROR_PM_UCP_NO_RECIPIENTS					(0x1604L | 0x20000000L)
#define ERROR_PM_UCP_INCOMPATIBLE_MESSAGE			(0x1605L | 0x20000000L)

/**	UCP transaction	*/
/*
const pmuint32 kPMUCPTransactionStartedError		= 0x1040006;
const pmuint32 kPMUCPTransactionNotStartedError		= 0x1040007;
const pmuint32 kPMUCPTransactionNoResultError		= 0x1040008;
const pmuint32 kPMUCPTransactionNoOperationError	= 0x1040009;
*/

#define ERROR_PM_UCP_TRANSACTION_STARTED			(0x1606L | 0x20000000L)
#define ERROR_PM_UCP_TRANSACTION_NOT_STARTED		(0x1607L | 0x20000000L)
#define ERROR_PM_UCP_TRANSACTION_NO_RESULT			(0x1608L | 0x20000000L)
#define ERROR_PM_UCP_TRANSACTION_NO_OPERATION		(0x1609L | 0x20000000L)

/**	This code is to be or-ed with kUCPNACKxxxx codes	*/
/*
const pmuint32 kPMUCPNACKError						= 0x1040100;
*/

#define ERROR_PM_UCP_NACK							(0x1700L | 0x20000000L)

/**	NACK codes	*/
/*
const pmint32 kUCPNACKChecksumError							= 1;
const pmint32 kUCPNACKSyntaxError							= 2;
const pmint32 kUCPNACKOperationNotSupported					= 3;
const pmint32 kUCPNACKOperationNotAllowed					= 4;
const pmint32 kUCPNACKCallBearingActive						= 5;
const pmint32 kUCPNACKInvalidAdC							= 6;
const pmint32 kUCPNACKAuthenticationFailure					= 7;
const pmint32 kUCPNACKAllCallsLegitimisationFailure			= 8;
const pmint32 kUCPNACKGANotValid							= 9;
const pmint32 kUCPNACKRepetitionNotAllowed					= 10;
const pmint32 kUCPNACKRepetitionLegitimisationFailure		= 11;
const pmint32 kUCPNACKPriorityCallNotAllowed				= 12;
const pmint32 kUCPNACKPriorityLegitimisationFailure			= 13;
const pmint32 kUCPNACKUrgentMessageNotAllowed				= 14;
const pmint32 kUCPNACKUrgentLegitimisationFailure			= 15;
const pmint32 kUCPNACKReverseChargingNotAllowed				= 16;
const pmint32 kUCPNACKReverseChargingLegitimisationFailure	= 17;
const pmint32 kUCPNACKDeferredDeliveryNotAllowed			= 18;
const pmint32 kUCPNACKNewACNotValid							= 19;
const pmint32 kUCPNACKNewLCNotValid							= 20;
const pmint32 kUCPNACKStandardTextNotValid					= 21;
const pmint32 kUCPNACKTimePeriodNotValid					= 22;
const pmint32 kUCPNACKMessageTypeNotSupported				= 23;
const pmint32 kUCPNACKMessageTooLong						= 24;
const pmint32 kUCPNACKRequestedStandardTextNotValid			= 25;
const pmint32 kUCPNACKMessageTypeNotValidForPagerType		= 26;
const pmint32 kUCPNACKMessageNotFoundInSMSC					= 27;
const pmint32 kUCPNACKSubsciberHandUp						= 30;
const pmint32 kUCPNACKFaxGroupNotSupported					= 31;
const pmint32 kUCPNACKFaxMessageTypeNotSupported			= 32;
const pmint32 kUCPNACKAddressAlreadyInList					= 33;
const pmint32 kUCPNACKAddressNotInList						= 34;
const pmint32 kUCPNACKListFull								= 35;

*/

//-----------------------------------------------------------------------
/**	Serial */
/*
const pmuint16 kPMSerialErrorClass = 0x200;

const pmuint32 kPMSerialUnknownError	= 0x2000001;
const pmuint32 kPMSerialReadWriteError	= 0x2000002;
const pmuint32 kPMSerialTimeOutError	= 0x2000003;
*/

#define ERROR_PM_SERIAL_UNKNOWN						(0x2000L | 0x20000000L)
#define ERROR_PM_SERIAL_READWRITE					(0x2001L | 0x20000000L)
#define ERROR_PM_SERIAL_TIMEOUT						(0x2002L | 0x20000000L)

/*
const pmuint32 kPMSerialMCTimeOutError	= 0x2000101;
const pmuint32 kPMSerialMCCommandModeError	= 0x2000102;
const pmuint32 kPMSerialMCResetError	= 0x2000103;
const pmuint32 kPMSerialMCInitError	= 0x2000104;
const pmuint32 kPMSerialMCHardFlowError	= 0x2000105;
const pmuint32 kPMSerialMCDialToError	= 0x2000106;
const pmuint32 kPMSerialMCHangUpError	= 0x2000107;
*/
#define ERROR_PM_SERIAL_MC_TIMEOUT					(0x2080L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_COMMANDMODE				(0x2081L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_RESET					(0x2082L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_INIT						(0x2083L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_HARDFLOW					(0x2084L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_DIALTO					(0x2085L | 0x20000000L)
#define ERROR_PM_SERIAL_MC_HANGUP					(0x2086L | 0x20000000L)

/*
const pmuint32 kPMSerialULCTimeOutError	= 0x2000201;
const pmuint32 kPMSerialULCLoginError	= 0x2000203;
const pmuint32 kPMSerialULCPasswordError	= 0x2000203;
*/
#define ERROR_PM_SERIAL_ULC_TIMEOUT					(0x2090L | 0x20000000L)
#define ERROR_PM_SERIAL_ULC_LOGIN					(0x2091L | 0x20000000L)
#define ERROR_PM_SERIAL_ULC_PASSWORD				(0x2092L | 0x20000000L)

/*
const pmuint32 kPMSerialKMCTimeOutError	= 0x2000301;
const pmuint32 kPMSerialKMCENQROMError	= 0x2000302;
const pmuint32 kPMSerialKMCSerError	= 0x2000303;
const pmuint32 kPMSerialKMCServiceError	= 0x2000304;
*/
#define ERROR_PM_SERIAL_KMC_TIMEOUT					(0x20A0L | 0x20000000L)
#define ERROR_PM_SERIAL_KMC_ENQROM					(0x20A1L | 0x20000000L)
#define ERROR_PM_SERIAL_KMC_SER						(0x20A2L | 0x20000000L)
#define ERROR_PM_SERIAL_KMC_SERVICE					(0x20A3L | 0x20000000L)

//-----------------------------------------------------------------------
#endif // PMWErr_h

