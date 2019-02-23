// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MyAnalysisDictReflexDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/global/projecta/projectdirs/atlas/wmccorma/TrkExclusiveWW/analysis/LowPtAnalysis/source/MyAnalysis/MyAnalysis/MyAnalysisDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *MyxAODAnalysis_Dictionary();
   static void MyxAODAnalysis_TClassManip(TClass*);
   static void delete_MyxAODAnalysis(void *p);
   static void deleteArray_MyxAODAnalysis(void *p);
   static void destruct_MyxAODAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MyxAODAnalysis*)
   {
      ::MyxAODAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MyxAODAnalysis));
      static ::ROOT::TGenericClassInfo 
         instance("MyxAODAnalysis", "MyxAODAnalysis.h", 9,
                  typeid(::MyxAODAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &MyxAODAnalysis_Dictionary, isa_proxy, 4,
                  sizeof(::MyxAODAnalysis) );
      instance.SetDelete(&delete_MyxAODAnalysis);
      instance.SetDeleteArray(&deleteArray_MyxAODAnalysis);
      instance.SetDestructor(&destruct_MyxAODAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MyxAODAnalysis*)
   {
      return GenerateInitInstanceLocal((::MyxAODAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::MyxAODAnalysis*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MyxAODAnalysis_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MyxAODAnalysis*)0x0)->GetClass();
      MyxAODAnalysis_TClassManip(theClass);
   return theClass;
   }

   static void MyxAODAnalysis_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MyxAODAnalysis(void *p) {
      delete ((::MyxAODAnalysis*)p);
   }
   static void deleteArray_MyxAODAnalysis(void *p) {
      delete [] ((::MyxAODAnalysis*)p);
   }
   static void destruct_MyxAODAnalysis(void *p) {
      typedef ::MyxAODAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MyxAODAnalysis

namespace {
  void TriggerDictionaryInitialization_libMyAnalysisDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libMyAnalysisDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$MyAnalysis/MyxAODAnalysis.h")))  MyxAODAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMyAnalysisDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef HAVE_PRETTY_FUNCTION
  #define HAVE_PRETTY_FUNCTION 1
#endif
#ifndef HAVE_64_BITS
  #define HAVE_64_BITS 1
#endif
#ifndef __IDENTIFIER_64BIT__
  #define __IDENTIFIER_64BIT__ 1
#endif
#ifndef ATLAS
  #define ATLAS 1
#endif
#ifndef ROOTCORE
  #define ROOTCORE 1
#endif
#ifndef XAOD_STANDALONE
  #define XAOD_STANDALONE 1
#endif
#ifndef XAOD_ANALYSIS
  #define XAOD_ANALYSIS 1
#endif
#ifndef ROOTCORE_RELEASE_SERIES
  #define ROOTCORE_RELEASE_SERIES 25
#endif
#ifndef PACKAGE_VERSION
  #define PACKAGE_VERSION "MyAnalysis-00-00-00"
#endif
#ifndef PACKAGE_VERSION_UQ
  #define PACKAGE_VERSION_UQ MyAnalysis-00-00-00
#endif
#ifndef EIGEN_DONT_VECTORIZE
  #define EIGEN_DONT_VECTORIZE 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef MYANALYSIS_MYANALYSIS_DICT_H
#define MYANALYSIS_MYANALYSIS_DICT_H

// This file includes all the header files that you need to create
// dictionaries for.

#include <MyAnalysis/MyxAODAnalysis.h>

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MyxAODAnalysis", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMyAnalysisDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMyAnalysisDict_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T, typename BASE> class DataVector;", 1}}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMyAnalysisDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMyAnalysisDict() {
  TriggerDictionaryInitialization_libMyAnalysisDict_Impl();
}
