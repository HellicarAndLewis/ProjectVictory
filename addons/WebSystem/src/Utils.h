#pragma once
#include "ofMain.h"
#include "Json.h"
#include "VideoFX.h"
#include "BaseEffect.h"

namespace WebSystem {
    namespace Utils {
        
        static const int countHashTagsInResult(Json::Value result) {
            if ( !result.isArray() ) {
                ofLogError() << "countHashTagsInResult passed result which is not array";
                ofLogError() << "result is " << result;
                return -1;
            }
            int total = 0;
            for (int i=0; i<result.size(); ++i) {
                if ( !result[i].isNull() ) {
                    total += ofToInt(result[i].asString());
                }
            }
            return total;
        }
        
        static const void applyPayload( VideoFX *videoFX, Json::Value payload ) {
            for (int i = 0;  i<payload["effects"].size(); ++i) {
                for (vector<BaseEffect*>::iterator it = videoFX->effects.begin(); it!= videoFX->effects.end(); ++it) {
                    if (payload["effects"][i]["name"] == (*it)->name) {
                        // apply the payload
                        (*it)->applyShaderMap(payload["effects"][i]["maps"]);
                    }
                }
            }
        }
        
    }
}