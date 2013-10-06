#pragma once
#include "ofMain.h"
#include "Json.h"
#include "VideoFX.h"
#include "BaseEffect.h"

namespace WebSystem {
    namespace Utils {
        
        // Returns the number of hasg tags in a JSON array
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
        
        // Applies a payload to the effects
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
        
        // Decays effect
        static const void decayEffects( VideoFX *videoFX, float lastDecayTime, float decayRatePerSecondScalar ) {
            
            float timeDelta = ofGetElapsedTimef() - lastDecayTime;

            for (vector<BaseEffect*>::iterator it = videoFX->effects.begin(); it!= videoFX->effects.end(); ++it) {
             
                Json::Value currentShaderMap = (*it)->getShaderMap();
                Json::Value defaultShaderMap = (*it)->getShaderDefaultMap();

                Json::Value::Members members = currentShaderMap.getMemberNames();
                for ( Json::Value::Members::iterator vit = members.begin();  vit != members.end(); ++vit ) {

                    float currentValue = currentShaderMap[ *vit ].asFloat();
                    float defaultValue = defaultShaderMap[ *vit ].asFloat();
                    float diff =  currentValue - defaultValue;

                    if ( diff > 0.0001 || diff > -0.001) {
                        float decayAmount = diff * decayRatePerSecondScalar * timeDelta;
                        currentShaderMap[ *vit ] = currentValue - decayAmount;
                    }

                }

                (*it)->applyShaderMap(currentShaderMap);
            }
            
        }
        
    }
}