#include <iostream>
#include <string>
#include <sstream>
#include <functional>

#include "gtest/gtest.h"
#include "../signality.hpp"
#include "../sign.stream.in.hpp"
#include "../fnc.control.hpp"
#include "../cfbind.hpp"

using namespace std;
using namespace signality;
using namespace cfbind;

long  __global_int=0;

bool __print=false;

bool fget_int_test( int&&v) {
    if (__print) {
        cout<<" get: "<<v<<endl;
    }
    __global_int=v;
    return true;
}

bool __bad=false;
bool __fail=false;

bool f_badfail(bool  bad) {
    if (bad)
        __bad=true;
    else
        __fail=true;
    cout << (bad? " -bad- " : "-fail")<<endl;
    return false;
}

void f_end() {
    cout<<"-end-"<<endl;
}


TEST (signality_test_case, reader__test)
{
    //cout<<"-1-"<<endl;
    istringstream ist("99");
    stream_in<int, istringstream > fsstr(ist);

    //cout<<"-2-"<<endl;
    auto r = fsstr(  fget_int_test, f_badfail);
    //cout<<"-3-"<<endl;
    EXPECT_EQ (__global_int==99,true);
}


istringstream  tgen_iss(unsigned int sz, const string comma=" " )

{

    ostringstream ost;
    unsigned int i = 1;

    while (i <= sz)
        ost << i++ << comma;
    return istringstream(ost.str());
};


long long vsumm=0;

bool add(int&&v) {

    if (v==7591) {
        cout<<"\n***\n";
    }

    vsumm+=v;




    if (__print) {
        cout << "\n-----v: "<< v << " vsumm: "<<vsumm;

    }
    return true;
}

TEST (signality_test_case, inmonade_reader__test)
{
    istringstream ist("99");
    stream_in<int, istringstream > fsstr(ist);
    auto r = fsstr();

    {

        auto ist=tgen_iss(4);
        stream_in<int, istringstream> fss(ist);

        cout<< ist.str()<<"\n";

        while (

            fss(add, f_badfail)) {

        };
        cout<< "\n: "<<vsumm<<endl;
        EXPECT_EQ(vsumm,10);
        vsumm=0;
    }

    {

        auto ist=tgen_iss(40202);
        stream_in<int, istringstream> fss(ist);

        cout<< ist.str()<<"\n";

        while (

            fss(add, f_badfail)) {

        };
        cout<< "\n: "<<vsumm<<endl;
        EXPECT_EQ(vsumm,808120503ull);
        vsumm=0;
    }
}

TEST(signality_test_case, inmonade_reader__fail)
{
    __bad=__fail=false;
    istringstream verybad("n??");
    //stream_in<int>fx(verybad);

    bool bend = stream_in<int, istream>(verybad,add, f_badfail);
    EXPECT_EQ(bend,false);
    EXPECT_EQ(__bad,false);
    EXPECT_EQ(__fail,true);
}


int __dw_test=0;

int add_dw() {
    return __dw_test<10 ?  ++__dw_test : 0;
}

TEST(fcontrol_tescase, do_while_test) {
    _control_<int>::_do_while_()( add_dw);
    EXPECT_EQ(__dw_test, 10);
}


TEST(fcontrol_tescase, while_do_test) {
    __dw_test=0;
    _control_<int>::_while_do_( []()->bool {return false;})( add_dw);
    EXPECT_EQ(__dw_test, 0);
}



TEST (signality_test_case, inmonade_reader__test2)
{


    __global_int=0;
    vsumm=0;
    auto ist=tgen_iss(8000);
    stream_in<int, istringstream> fss(ist);

    cout<< ist.str()<<"\n";
    __print=true;
    //  _control_<int>::_do_while_()(fss(add, f_badfail)) );
    bool btest=    fss._while_until_fail_( add, f_badfail ) ;


    cout<< "\n: "<<vsumm<<endl;
    //  EXPECT_EQ(vsumm,808120503ull);
    vsumm=0;

}


bool add_break_at1000(int&&v) {

    if (v>=1000)
        return false;
    vsumm+=v;

    if (__print) {
        cout << "\n-----v: "<< v << " vsumm: "<<vsumm;
    }
    return true;
}

bool add_break_at1000(int v) {

    if (v>=1000)
        return false;
    vsumm+=v;

    if (__print) {
        cout << "\n-----v: "<< v << " vsumm: "<<vsumm;
    }
    return true;
}

TEST (signality_test_case, inmonade_reader__test_consumefail)
{
    __print=true;
    cout<< "test_consumefail";
    auto ist=tgen_iss(40202);

    stream_in<int, istringstream > fsstr(ist);

    bool btest=  fsstr._while_until_fail_( add_break_at1000, f_badfail ) ;


    __print=false;
}




/*
TEST (signality_test_case, inmonade_reader__rekurs_test)
{


        __global_int=0;
        vsumm=0;
        auto ist=tgen_iss(8000);
        stream_in<int, istringstream> fss(ist);

        cout<< ist.str()<<"\n";
        __print=true;
      //  _control_<int>::_do_while_()(fss(add, f_badfail)) );
        bool btest=    fss._until_fail_( add, f_badfail ) ;

        cout<< "\n: "<<vsumm<<endl;
      //  EXPECT_EQ(vsumm,808120503ull);
        vsumm=0;

}*/

struct test_reader_object {
    long long  _v=0;
    istringstream& _ist;
    //badfail
    test_reader_object(istringstream& ist):_ist(ist) {}
    bool operator () (bool bad) const {
        if (!bad) {
            cout<<"\nautsch!\n";
            _ist.clear();
            string s;
            _ist>>s;
            return true;
        }
        else {
            cout << "\ncrash!!\n";
            // _v-=10;
        }
        return bad;
    }

    //consume
    bool operator()(int&&v)   {
        _v+=v;
        return true;
    }

    //end
    void operator()() {
        cout<<"\n\n==end==\n\n";
    }


};


istringstream  tgen_iss_defect(unsigned int sz, int moddefect=0, const string comma=" " )

{

    ostringstream ost;
    unsigned int i = 1;

    while (i <= sz) {
        if (moddefect && (i%moddefect==0))
            ost << " x"<< comma;
        ost << i++ << comma;
    }
    return istringstream(ost.str());
};



//    typedef  ref<test_reader_object>   OType;
typedef  test_reader_object& OType;

TEST (signality_test_case, inmonade_reader__object_test)
{
    auto ist=tgen_iss_defect(40202,123);

    test_reader_object reader_object(ist);;

    stream_in<int, istringstream, OType > fsstr(ist);

    bool btest=  fsstr._while_until_fail_(
                     _F1<bool,int&&>::call<   OType  >(   reader_object ),
                     _F1<bool, bool>::call<  OType>( reader_object)
                 ) ;
    cout<<" \nS: "<< reader_object._v<<"\n";

}


int f1() {
    return 1;
}

int f11(int v) {
    return 1;
}



struct sf2 {
    int operator()() const {
        return 2;
    }
};


struct sf21 {
    int operator()(int v) const {
        return v+2;
    }
};


struct vv0 {
    int _v=100;
    int operator()() {
        return _v;
    }
};

struct vv1 {
    int _v=100;
    int operator()(int v) const {
        return _v+v;
    }
};

////impl
/*

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
        Ret operator()() {
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

        Ret operator()() {
            return _c();
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
        Ret operator()(Val v) {
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

        Ret operator()(Val v) {
            return _c(v);
        }
    };
};

*/



TEST (signality_test_case, cfbin_test)
{


    //
    auto r1=  _F0<int>::call<>(f1)();
    EXPECT_EQ( r1,1 );


    auto r2= _F0<int>::call<sf2>()();
    EXPECT_EQ( r2,2 );

    vv0 v99 {99};


    auto r3= _F0<int>::call< vv0&  >(v99)();
    EXPECT_EQ( r3,99 );

    auto r11=  _F1<int>::call<>(f11)(5);
    EXPECT_EQ( r11,1 );

    auto r12= _F1<int>::call<vv1>()(5);
    EXPECT_EQ( r12,105 );

    vv1 v200 {200};
    auto r13= _F1<int>::call<vv1&>(v200)(50);
    EXPECT_EQ( r12,250 );



}
