
#ifndef _SIGNALITY_SSI_HPP__
#define _SIGNALITY_SSI_HPP__ 1

#include <fstream>
#include <iostream>
#include <boost/variant.hpp>

#include "signality.hpp"
#include  "cfbind.hpp"
#include "inline_variant.hpp"

using namespace std;
using namespace cfbind;

namespace signality {
struct eof {};
struct bad {};
struct fail {};
template  <typename E, typename S=fstream, typename Reader= _undefined>
struct    stream_in {
    S& _ist;
    //      using boost::variant;
    stream_in(S&  ist):_ist(ist) {}
    typedef  boost::variant<E, bad,fail, eof> _R;

    const _R operator() () const {
        if (_ist.good()) {
            E v;
            _ist>>v;
            if (_ist.bad())
                return bad();
            if (_ist.fail()&& !_ist.eof())
                return fail();
            return  move(v);
        }
        if (_ist.eof())
            return eof();
        if (_ist.fail())
            return fail();
        return bad();
    }


    bool operator()( typename  signality::stream_element <E,Reader>:: FConsume fc, typename   signality::stream_element<E,Reader>::FBadFail fbf=nullptr  ) {
        //  bool beof= operator()();//TEST
        auto r = operator()();
        return match(r,
                     [=] (bad)->bool { return  (fbf) ? fbf (true) : false;  },
                     [=] (fail)->bool { return (fbf)? fbf(false) : false;  },
                     [=] (eof)->bool { return  false;  },
                     [=] (E v)->bool {   fc(move(v) ) ;  return  true;  }
                    );
    }
    
    /*
     * rekurs fails <~8000 positions
    bool _until_fail_( typename  signality::stream_element <E>:: FConsume fc, typename   signality::stream_element<E>::FBadFail fbf=nullptr  ) {
        //  bool beof= operator()();//TEST
        auto r = operator()();
        return match(r,
                     [=] (bad)->bool { 
                         return 
                             ( fbf != nullptr) ? 
                                     (  (*fbf) (true))  ?
                                        _until_fail_(fc, fbf) :  false                                
                                        : false;
                         
                      },
                     [=] (fail)->bool { 
                         return 
                                    ( fbf != nullptr) ? 
                                     (  (*fbf) (false))  ?
                                        _until_fail_(fc, fbf) :  false                                
                                        : false;
                    },
                     [=] (eof)->bool { return  false;  },
                     [=] (E v)->bool {  if (fc) fc(move(v)  ) ;  return   _until_fail_(fc, fbf);  }
                    );
    }
    */
    
    bool _while_until_fail_( typename  signality::stream_element <E,Reader>:: FConsume fc, typename   signality::stream_element<E,Reader>::FBadFail fbf=nullptr  ) {
        bool b=false;
        do{
        auto r = operator()();
        b =match(r,
                     [=] (bad)->bool { 
                         return  ( fbf)  &&  fbf (true)  ;
                                 
                         
                      },
                     [=] (fail)->bool { 
                                    return    fbf  &&   fbf (false ) ;
                         
                    },
                     [=] (eof)->bool {  return false;  },
                     [=] (E v)->bool {  if (fc) fc(move(v)  ) ;  return true; }
                    );
        }
        while (b);
        return true;
    }
    
    bool _while_until_consume_fail_( typename  signality::stream_element <E, Reader>:: FConsume fc, typename   signality::stream_element<E,Reader>::FBadFail fbf=nullptr  ) {
        bool b=false;
        do{
        auto r = operator()();
        b =match(r,
                     [=] (bad)->bool { 
                         return
                            ( fbf != nullptr) ? 
                                     (  (*fbf) (true))  ?
                                         _while_until_fail_(fc, fbf) :    false                                
                                        :  false;
                         
                      },
                     [=] (fail)->bool { 
                                    return 
                                   ( fbf != nullptr) ? 
                                     (  (*fbf) (false))  ?
                                          _while_until_fail_(fc, fbf) :   false                                
                                        : false;
                         
                    },
                     [=] (eof)->bool {  return false;  },
                     [=] (E v)->bool {  if (fc)   
                                            return fc(move(v)  ) ;  
                                        else
                                            return false;  }
                    );
        }
        while (b);
        return b;
    }
    
    
    bool _result =false;
    
    stream_in(S&  ist,  typename signality::stream_element <E,Reader>::FConsume fc,  typename signality::stream_element <E,Reader>::FBadFail fbf=nullptr ):_ist(ist) {
        _result = operator()(fc,fbf);
    }
    
    operator bool(){ return _result;}
    
};



}



#endif
