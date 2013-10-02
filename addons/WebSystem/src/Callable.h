#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

namespace WebSystem {
    
    // Callable allows for a basic JSON event system
    
    struct Callable {
        virtual ~Callable () {};
        virtual void operator()(Json::Value body) = 0;
    };
    
    template <class Object>
    struct TemplateCallable : public Callable {
        typedef void (Object::*MemberFunction)(Json::Value body);
        TemplateCallable(Object* obj, MemberFunction mem_fun): _obj(obj),_mem_fun(mem_fun){}
        virtual void operator() (Json::Value body) {
            ((_obj)->*(_mem_fun))(body);
        }
        Object* _obj;
        MemberFunction _mem_fun;
    };
    
}

