#ifndef  __FNC__CONTROL__HPP__
#define  __FNC__CONTROL__HPP__  1


template <typename Res>
struct _control_
{
    typedef Res(*FncToCall)();
    typedef bool (*FEvalResult)(const Res&);
    typedef bool (*FTest)();
struct _do_while_
{
    _do_while_( FncToCall ftocall)
    {
        _fevalResult=[](const Res& r)->bool { return r!= 0;}  ;
        operator()( ftocall);
    }
    
    _do_while_( FncToCall ftocall, FEvalResult fevalResult)
    {
        _fevalResult= fevalResult;
        
        operator()( ftocall);
    }
    
    FEvalResult _fevalResult;
    _do_while_(  )
    {
          _fevalResult=[](const Res& r)->bool { return r!= 0;} ;
    }
    
    void operator()(FncToCall ftocall,   FEvalResult feval){
        do{
            auto res= ftocall();
            if (!_fevalResult(res))
                return;
        }while (true);
    }
    
    void operator()(FncToCall ftocall){
        do{
            auto res= ftocall();
            if (!_fevalResult(res))
                return;
        }while (true);        
    }
};
    
    struct _while_do_
{
    
    FTest  _fTest;
    _while_do_(   FTest ftest )
    {
        
        _fTest= ftest;
    }
    
    void operator()(FncToCall ftocall){
        do{
            assert(_fTest);
            if (!_fTest())
                return;
            ftocall();
            
        }while (true);
    }
    
    _while_do_(FncToCall ftocall, FTest  fTest)
    {
        _fTest = fTest;        
         operator()(ftocall);        
    }
    
    /*
    void operator()(FncToCall ftocall){
        do{
            auto res= ftocall();
            if (!_fevalResult(res))
                return;
        }while (true);        
    }*/
    
};


};



#endif
