// ===========================================================================
//  PMTrace.h
//  Copyright (C) 1996-97 AbyreSoft, Inc. All rights reserved.
// ===========================================================================

#ifndef PMTrace_h
#define PMTrace_h

// ---------------------------------------------------------------------------

#include "PMRC.h"
#include "PMCriSec.h"
#include <stdarg.h>

// ---------------------------------------------------------------------------

    /** Type used to specify trace module names.    */
typedef pmuint16 PMTrace_Module;

    /** Type used to specify trace codes.   */
typedef pmuint16 PMTrace_Code;

    /** Type used to specify trace identification.  */
typedef pmuint32 PMTrace_Ident;

    /** Type used to specify trace kinds.   */
typedef pmuint16 PMTrace_eKind;

    /** Trace kind: Normal (default).   */
const PMTrace_eKind PMTrace_kKindNormal = 0;

    /** Trace kind: End of line trace (EOL).    */
const PMTrace_eKind PMTrace_kKindEOT = 1;

    /** Trace kind: Buffer (binary).    */
const PMTrace_eKind PMTrace_kKindBinary = 2; 

// ---------------------------------------------------------------------------

PMDEFINE(PMTracer);

// ===========================================================================
//  PMTraceSendParams
// ===========================================================================

    /** Internal use.   */
struct PMTraceSendParams
{
    PMTrace_eKind   itsKind;
    PMTrace_Ident   itsIdent;
    const char*     itsFile;
    pmuint32        itsLine;
    void*           itsData;
    size_t          itsDataLen;
    pmuint32        itsIndentLevel;
    pmuint32        itsTimeStamp;

    PMTraceSendParams();
    ~PMTraceSendParams();
};

// ===========================================================================
//  PMTracer
// ===========================================================================
/** Base class for tracers. */

class PMTracer : public PMRC
{
public:
        /** Constructor.    */
    PMTracer();
    
        /** Destructor. */
    virtual ~PMTracer();

        /**
        PMTracer::DisplayAssert provides a default implementation (which calls Assert()).
        The fields itsIdent of PMTraceSendParams* is not valid.
        DisplayAssert must delete aParams before throwing in order to avoid memory leaks.
        */
    virtual void        DisplayAssert(const char* aTest, PMTraceSendParams* aMessage);

        /**
        When an assertion is raised, the assertion is processed by the active
        tracer which then calls this method. In Release mode this method is 
        called directly.
        This method's default processing is to call the 'pm_throw_assert' stub function (even
        if traces are not active).
        */
    virtual void        Assert();

        /** Returns the current tracer. */
    static PMTracerRef  GetTracer();
    
        /** Sets 'aTracer' as the current one.  */
    static void         SetTracer(const PMTracerRef &aTracer);

        /**
        Sets whether the tracer is active or not. The active flag is a counted boolean value.
        This means that each call to SetActive(true) must be balanced by a call to
        SetActive(pmfalse) to stop trace display.
        */
    static pmbool       SetActive(pmbool afActive);
    
        /** Returns if the tracer is active (i.e. if the tracer is currently displaying traces).    */
    static pmbool       IsActive();

        /**
        The five following methods allocate their return values.
        If you use them directly (not using macros related to PM_TRACE and PM_ASSERT)
        you must delete the return value yourself (using 'operator delete').
        */
    virtual PMTraceSendParams*  T(const char* aFormat, ...);
    virtual PMTraceSendParams*  TL(const char* aFormat, ...);
    virtual PMTraceSendParams*  B(const void* aBuffer, size_t aSize);

    virtual PMTraceSendParams*  VT(const char* aFormat, va_list anArgs);
    virtual PMTraceSendParams*  VTL(const char* aFormat, va_list anArgs);

        /**
        Using this pointer when EOL characters are needed to display traces will
        prevent fatal deadlock between PMTrace, PMStr, and PMRC.
        */
    static const char*  PlatformEOL();

        /** Internal use. Tells the tracer to display data in 'aParams'.    */
    virtual void    Send(PMTraceSendParams* aParams) = 0;

    static inline PMCriticalSection*    GetCriticalSection() { return &cCritSect; }

protected:
    static PMTracerRef*         cCurrentTracer;
    static pmint32              cfActive;
    static PMCriticalSection    cCritSect;

    static void         SetupTracer();


private:
    PMTracer(const PMTracer&);
    PMTracer& operator=(const PMTracer&);
};

// ===========================================================================
//  PMTraceActiver
// ===========================================================================
/**
Used to temporarily activate or deactivate trace display.
It must be instanciated as an automatic object.
*/

class PMTraceActiver
{
public:
        /** Constructor. Call PMTracer::SetActive(afActive).    */
    PMTraceActiver(pmbool afActive);
    
        /** Destructor. Call PMTracer::SetActive(!afActive).    */
    ~PMTraceActiver();

protected:
    pmbool  itsfActive;

private:
    PMTraceActiver();
    PMTraceActiver(const PMTraceActiver&);
    PMTraceActiver& operator=(const PMTraceActiver&);
};

// ===========================================================================
//  PMTraceIndenter
// ===========================================================================
/**
Add indentation to trace display for easier reading. 
Depending on the current tracer implementation, it may or may not use this 
to display traces.
When you need to add an indentation level, create an automatic instance of
PMTraceIndenter. When this automatic object is deleted, the indentation
level is removed.
The easiest way to use this class it to simply put the macro PM_TRACEINDENTER at the 
beginning of the block for which you want the traces be indented.
*/

class PMTraceIndenter
{
public:
        /** Constructor. Adds one level of indentation. */
    PMTraceIndenter();

        /** Destructor. Removes one level of indentation.   */
    ~PMTraceIndenter();

        /** Returns the current indentation level.  */
    static pmuint32 GetLevel();

protected:
    static pmuint32 cLevel;

private:
    PMTraceIndenter(const PMTraceIndenter&);
    PMTraceIndenter& operator=(const PMTraceIndenter&);
};

// ===========================================================================
//  PM_TRACE
//  PM_TRACEINDENTER
//  PM_API
// ===========================================================================

// ---------------------------------------------------------------------------
/*
Each file should begin with 
#define PM_TRACEMODULE MIME.
You can use #define PM_TRACEMODULE TEMPORARY, which
implies that PMTrace_kMIMEModule is defined in PMTrace.h.
PM_DEFINE_TRACE_CODE(Info, 1) -> PMTrace_kMIMEInfoCode
*/

#define PM_TRACEMODULETOKEN2(module)        PMTrace_k ## module ##Module
#define PM_TRACEMODULETOKEN1(module)        PM_TRACEMODULETOKEN2(module)
#define PM_TRACEMODULETOKEN                 PM_TRACEMODULETOKEN1(PM_TRACEMODULE)

#define PM_TRACECODETOKEN2(code, module)    PMTrace_k ## module ## code ##Code
#define PM_TRACECODETOKEN1(code, module)    PM_TRACECODETOKEN2(code, module)
#define PM_TRACECODETOKEN(code)             PM_TRACECODETOKEN1(code, PM_TRACEMODULE)

/**
    Defines a new trace module.
    Standard trace modules are defined in PMTrace.h.
    You should use them instead of creating new modules as predefined modules
    will be correctly displayed by ObjectSet Reporter.
    See also: ObjectSet overview in WORD format for further explanations about traces.
*/
#define PM_DEFINE_TRACE_MODULE(module,value)    const PMTrace_Module PM_TRACEMODULETOKEN1(module) = value

/** 
    Defines a new trace code. 
    Although the are ignored you must define them before using traces. 
    They will be used in a next version of ObjectSet Reporter. They will be used to
    provide a better granularity inside one trace module.
    You must define trace codes in your own source files.
    In a .cpp file in which you want to send traces you must add the following code at the beginning:.

    #include "PMTrace.h".
    #define PM_TRACEMODULE STRUCT.
    PM_DEFINE_TRACE_CODE(Info, 1);.

    See also: ObjectSet overview in WORD format for further explanations about traces.
*/
#define PM_DEFINE_TRACE_CODE(code,value)        static const PMTrace_Code PM_TRACECODETOKEN(code) = value

#define PM_INTERNAL_REPORT(aCode, X)                                        \
    do                                                                      \
    {                                                                       \
        PMCriticalSectionLocker theLocker(PMTracer::GetCriticalSection());  \
        if (PMTracer::IsActive())                                           \
        {                                                                   \
            PMTracerRef         _xTracer_ = PMTracer::GetTracer();          \
            PMTraceSendParams*  _xParams_ = _xTracer_->X;                   \
            if (_xParams_ != 0)                                             \
            {                                                               \
                _xParams_->itsIdent = (((pmuint32) PM_TRACEMODULETOKEN) << 16) + aCode; \
                _xParams_->itsFile = __FILE__;                              \
                _xParams_->itsLine = __LINE__;                              \
                _xParams_->itsIndentLevel = PMTraceIndenter::GetLevel();    \
                _xParams_->itsTimeStamp = pm_system_time();                 \
            }                                                               \
            _xTracer_->Send(_xParams_);                                     \
            delete _xParams_;                                               \
        }                                                                   \
    } while (0)

/**
    This macro acts like \Ref{PM_TRACE} but it is not removed when building 
    in release mode.
*/
#define PM_REPORT(aCode, X) \
    PM_INTERNAL_REPORT(PM_TRACECODETOKEN(aCode), X)

// ---------------------------------------------------------------------------

#ifdef PMF_NO_TRACE

    /** Release version: does nothing. */
    #define PM_TRACE(code, X)   ((void) 0)
    /** Release version: does nothing. */
    #define PM_TRACEINDENTER    ((void) 0)
    
    /** Release version: does nothing. */
    #define PM_API(F)           ((void) 0)

#else   // PMF_NO_TRACE

    /** 
        Debug version: generates a trace.
        See Netcore.doc overview in WORD format for explanations on traces.
    */
    #define PM_TRACE(aCode, X)  PM_REPORT(aCode, X)
    /** Debug version: indents traces: all traces in the scope of this expression will be right-indented. */ 
    #define PM_TRACEINDENTER    PMTraceIndenter _xIndent##__LINE__
    
    #ifdef PMF_PMAPI
        #define PM_API(X)       PM_INTERNAL_REPORT(PMTrace_kAPICode, X)
    #else
        #define PM_API(X)       ((void) 0)
    #endif

#endif  // PMF_NO_TRACE

// ===========================================================================
//  PM_ASSERT
// ===========================================================================

#ifdef PMF_RELEASE_ASSERT

    /** 
        Release version: Asserts without tracing the failed condition.
        See Netcore.doc for further explanations.
    */
    #define PM_ASSERT(aCondition, X)                                \
        do                                                          \
        {                                                           \
            if (!(aCondition))                                      \
            {                                                       \
                PMCriticalSectionLocker theLocker(PMTracer::GetCriticalSection());  \
                {                                                   \
                    PMTracer::GetTracer()->Assert();                \
                }                                                   \
            }                                                       \
        } while (0)

    /** 
        Release version: Asserts without any condition and without any trace.
        See Netcore.doc for further explanations.
    */
    #define PM_ASSERT_NOW(X)                                        \
        do                                                          \
        {                                                           \
            PMCriticalSectionLocker theLocker(PMTracer::GetCriticalSection());  \
            {                                                       \
                PMTracer::GetTracer()->Assert();                    \
            }                                                       \
        } while (0)

#else   // PMF_RELEASE_ASSERT

    /** 
        Debug version: Asserts and traces the failed condition.
        See Netcore.doc for further explanations.
    */
    #define PM_ASSERT(aCondition, X)                                    \
        do                                                              \
        {                                                               \
            if (!(aCondition))                                          \
            {                                                           \
                PMCriticalSectionLocker theLocker(PMTracer::GetCriticalSection());  \
                {                                                       \
                    PMTracerRef _xTracer_ = PMTracer::GetTracer();      \
                    PMTraceSendParams*  _xParams_ = _xTracer_->X;       \
                    if (_xParams_ != 0)                                 \
                    {                                                   \
                        _xParams_->itsFile = __FILE__;                  \
                        _xParams_->itsLine = __LINE__;                  \
                    }                                                   \
                    _xTracer_->DisplayAssert(#aCondition, _xParams_);   \
                }                                                       \
            }                                                           \
        } while (0)

    /** 
        Debug version: Asserts without any condition but traces the file and line where it occurs.
        See Netcore.doc for further explanations.
    */
    #define PM_ASSERT_NOW(X)                                        \
        do                                                          \
        {                                                           \
            PMCriticalSectionLocker theLocker(PMTracer::GetCriticalSection());  \
            {                                                       \
                PMTracerRef _xTracer_ = PMTracer::GetTracer();      \
                PMTraceSendParams*  _xParams_ = _xTracer_->X;       \
                if (_xParams_ != 0)                                 \
                {                                                   \
                    _xParams_->itsFile = __FILE__;                  \
                    _xParams_->itsLine = __LINE__;                  \
                }                                                   \
                _xTracer_->DisplayAssert("", _xParams_);            \
            }                                                       \
        } while (0)

#endif  // PMF_RELEASE_ASSERT

// ===========================================================================
//  PM_DEFINE_TRACE_MODULE
// ===========================================================================

// ---------------------------------------------------------------------------
/** Unofficial trace module names.  */

PM_DEFINE_TRACE_MODULE(TEMPORARY,1);
PM_DEFINE_TRACE_MODULE(UNKNOWN,2);
PM_DEFINE_TRACE_MODULE(TEST,3);
PM_DEFINE_TRACE_MODULE(THREAD,4);
PM_DEFINE_TRACE_MODULE(FS,5);

const PMTrace_Code  PMTrace_kAPICode = 32768;

// ---------------------------------------------------------------------------
/** User module names.  */

PM_DEFINE_TRACE_MODULE(USER0, 16384);
PM_DEFINE_TRACE_MODULE(USER1, 16385);
PM_DEFINE_TRACE_MODULE(USER2, 16386);
PM_DEFINE_TRACE_MODULE(USER3, 16387);
PM_DEFINE_TRACE_MODULE(USER4, 16388);
PM_DEFINE_TRACE_MODULE(USER5, 16389);
PM_DEFINE_TRACE_MODULE(USER6, 16390);
PM_DEFINE_TRACE_MODULE(USER7, 16391);
PM_DEFINE_TRACE_MODULE(USER8, 16392);
PM_DEFINE_TRACE_MODULE(USER9, 16393);
PM_DEFINE_TRACE_MODULE(USER10, 16394);
PM_DEFINE_TRACE_MODULE(USER11, 16395);
PM_DEFINE_TRACE_MODULE(USER12, 16396);
PM_DEFINE_TRACE_MODULE(USER13, 16397);
PM_DEFINE_TRACE_MODULE(USER14, 16398);
PM_DEFINE_TRACE_MODULE(USER15, 16399);

// ---------------------------------------------------------------------------
// If you want to define trace module for your application you should use
// values greater or equal to 16384.
// ObjectSet will never use values in this range. If you use values less than
// 16384 you may have conflict with ObjectSet Trace Modules.
// 16384 To 16384+15 are pre-defined above for ease of use.

// ---------------------------------------------------------------------------
/** Trace module names. */

// Netcore
PM_DEFINE_TRACE_MODULE(RC, 0x0100);
PM_DEFINE_TRACE_MODULE(STRUCT, 0x0101);
PM_DEFINE_TRACE_MODULE(STATS, 0x0102);
PM_DEFINE_TRACE_MODULE(PROFILE, 0x0103);
PM_DEFINE_TRACE_MODULE(MEDIA, 0x0104);
PM_DEFINE_TRACE_MODULE(STREAM, 0x0105);
PM_DEFINE_TRACE_MODULE(CODER, 0x0106);
PM_DEFINE_TRACE_MODULE(DATA, 0x0107);
PM_DEFINE_TRACE_MODULE(SOCKET, 0x0108);
PM_DEFINE_TRACE_MODULE(INPR, 0x0109);

// Tools
PM_DEFINE_TRACE_MODULE(MIME, 0x0200);
PM_DEFINE_TRACE_MODULE(MODEM, 0x0201);
PM_DEFINE_TRACE_MODULE(URL, 0x0202);
PM_DEFINE_TRACE_MODULE(HTML, 0x0203);
PM_DEFINE_TRACE_MODULE(SGML, 0x0204);

// Protocols
PM_DEFINE_TRACE_MODULE(POP3, 0x0300);
PM_DEFINE_TRACE_MODULE(SMTP, 0x0301);
PM_DEFINE_TRACE_MODULE(IMAP, 0x0302);
PM_DEFINE_TRACE_MODULE(FTP , 0x0303);
PM_DEFINE_TRACE_MODULE(LDAP, 0x0304);
PM_DEFINE_TRACE_MODULE(HTTP, 0x0305);
PM_DEFINE_TRACE_MODULE(NNTP, 0x0306);
PM_DEFINE_TRACE_MODULE(UCP, 0x0307);

// ---------------------------------------------------------------------------

#endif // PMTrace_h
