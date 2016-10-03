#ifndef __CALL_FNC_BIND_HPP__
#define __CALL_FNC_BIND_HPP__   1

#include <type_traits>

namespace cfbind
{

//functions:

template <typename Ret  >
struct _F0
{
    struct _fcall_
    {
        typedef Ret(*Fnc)();
        Fnc _fnc;
        _fcall_(Fnc f) {
            _fnc=f;
        }
        Ret operator()() const {
            return _fnc();
        }
    };

    template <typename C= _fcall_>
    struct call
    {
        C _c;
//        call(  con_c(st C& c ){c=c;}
        call(  C c ): _c(c) {}
        call() :_c(C()) {}
        call( typename   _fcall_::Fnc  f):_c(C(f)) {}

        Ret operator()() const {
            return _c();
        }
        /*        operator typename _fcall_::Fnc () {
                    return _c;
                }
        */
        operator bool ()const {
            return true;   //spezifizieren nullptr
        }
    };
};

template <typename Ret, typename Val=Ret  >
struct _F1
{
    struct _fcall_
    {
        typedef Ret(*Fnc)(Val);
        Fnc _fnc;
        _fcall_(Fnc f) {
            _fnc=f;
        }

        Ret operator()(Val v)  const  {
            return _fnc( std::move(v));

        }


        /*
                  template<class Q = Val>
                  typename std::enable_if<std::is_rvalue_reference <Q>::value, Ret>::type operator()(Q v) const
                 {
                    return _fnc(std::move(v));
                 }

                 template<class Q = Val>
                 typename std::enable_if<!std::is_rvalue_reference<Q>::value  , Ret>::type operator() (Q v) const
                 {
                         return _fnc(std::move(v));
                 }*/

    };

    template <typename C= _fcall_>
    struct call
    {
        C _c;
        call(  C c ):_c(c) {}
        call() :_c(C()) {}

        call( typename   _fcall_::Fnc  f):_c(C(f)) {}

        Ret operator() (Val v)const {
            return _c(std::move((Val)v));
        }

        operator   bool ()const {
            return true;   //spezifizieren nullptr
        }
    };
};

//signals: no return

struct _S0
{
    struct _fcall_
    {
        typedef void(*Fnc)();
        Fnc _fnc;
        _fcall_(Fnc f) {
            _fnc=f;
        }
        void operator()() const {
            return _fnc();
        }
    };

    template <typename C= _fcall_>
    struct call
    {
        C _c;
//        call(  con_c(st C& c ){c=c;}
        call(  C c ): _c(c) {}
        call() :_c(C()) {}
        call( typename   _fcall_::Fnc  f):_c(C(f)) {}

        void operator()() {
            return _c();
        }
        operator bool ()const {
            return true;   //spezifizieren nullptr
        }
    };
};

template < typename Val >
struct _S1
{
    struct _fcall_
    {
        typedef void(*Fnc)(Val);
        Fnc _fnc;
        _fcall_(Fnc f) {
            _fnc=f;
        }
        void operator()(Val v) const {
            return _fnc( v);
        }
    };

    template <typename C= _fcall_>
    struct call
    {
        C _c;
        call(  C c ):_c(c) {}
        call() :_c(C()) {}

        call( typename   _fcall_::Fnc  f):_c(C(f)) {}

        void operator()(Val v) const {
            return _c(v);
        }
        operator  bool ()const   {
            return true;   //spezifizieren nullptr
        }
    };

};

struct _undefined {};


// _return
/*
template <typename T>
struct is_undefined{
    static const bool value=false;
};

template <_undefined>
struct is_undefined{
   static is_undefined  bool value=true;
};*/

/*

   template<class T>
         typename std::enable_if<std::is_pointer <T>::value, T&>::type _callable_ref(T  o)
        {
           return *o;
        }

         template<class T>
         typename std::enable_if<!std::is_pointer <T>::value, T>::type& _callable_ref(T  o)
        {
           return *o;
        }


*/

}

#endif
