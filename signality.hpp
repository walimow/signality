#ifndef _SIGNALITY_HPP__
#define _SIGNALITY_HPP__ 1

#include <fstream>
#include <iostream>

#define __TEST_FUNCOP__ 1


#include "cfbind.hpp"
using namespace cfbind;



using namespace std;

namespace signality {



#ifdef    __TEST_FUNCOP__
template<typename E  , typename Reader>
struct stream_element {

    typedef  typename   _F1<bool,  E&&>:: template  call<Reader>  FConsume;
    typedef   typename  _S1<  E>::template call<Reader>  FOk;

    typedef _F1<bool, bool>::template call<Reader> FBadFail;
    typedef _S0::template call<Reader> FEnd;  //typedef void(*FEnd)();

};
template <typename E>

struct stream_element <E,  _undefined> {

    typedef  typename   _F1<bool,  E&&>:: template  call<>  FConsume;
    typedef   typename  _S1<  E>::template call<>  FOk;
    typedef _F1<bool, bool>::call<> FBadFail;
    typedef _S0::call<> FEnd;  //typedef void(*FEnd)();
};


#else
template<typename E>
struct stream_element {
    typedef  bool (*FConsume)(E&&);
    typedef void(*FOk)(const E&);

    typedef bool(*FBadFail)(bool);

    typedef void(*FEnd)();

};
#endif


}


#endif


