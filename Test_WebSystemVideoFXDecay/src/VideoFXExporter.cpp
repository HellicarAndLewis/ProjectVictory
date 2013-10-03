#include "VideoFXExporter.h"

#pragma mark - GUI

void VideoFXExporter::createGUI() {
    if (!videoFx) { cout << "VideoFXExporter::createGUI add videoFx pointer before creating ui" << endl; return; }
    
    static bool hasCreatedGUI = false;
    if (hasCreatedGUI) { return; }
    hasCreatedGUI = true;

    // Create the GUI
    exporterGUI = new ofxUISuperCanvas( "VFX EXPORTER", 230, 23, 300, 200 );
    exporterGUI->setColorBack( ofColor( ofColor::blue, 150 ) );
    exporterGUI->addSpacer();
    
    exporterGUI->addLabel("MAPS");
    
    vector<BaseEffect*> effects = videoFx->effects;
    for ( int i=0; i<effects.size(); ++i) {
        // Add a label for each effect
        exporterGUI->addLabel(ofToUpper(effects[i]->name), OFX_UI_FONT_SMALL);
        ShaderMap map = effects[i]->getShaderMap();

        // Add a toggle for each map
        int nextDown = -1;
        Json::Value::Members members = map.getMemberNames();
        for ( Json::Value::Members::iterator it=members.begin();  it!=members.end(); ++it ) {
            EffectMapToggle *toggle = new EffectMapToggle(effects[i]->name, *it);
            // Add this effect to the map of toggles
            effectToggles[effects[i]->name][*it] = toggle;
            if (++nextDown % 3 == 0) {
                exporterGUI->addWidgetDown(toggle);
            } else {
                exporterGUI->addWidgetRight(toggle);
            }
        }
        exporterGUI->addSpacer();
    }
    
    // Add filename
    exporterGUI->addLabel("PAYLOAD NAME", OFX_UI_FONT_SMALL);
    payloadNameTextInput = exporterGUI->addTextInput("FilenameTextInput", "preset");
    
    // Commands
    exporterGUI->addLabel("COMMANDS", OFX_UI_FONT_SMALL);
    commandTextInput = exporterGUI->addTextInput("CommandTextInput", "command");
    
    // Add save button
    exporterGUI->addLabelButton("EXPORT PAYLOAD", false);  //new ofxUILabelButton(0,0, 200.0f, false, "EXPORT
    exporterGUI->addSpacer();
    exporterGUI->addLabelButton("EXPORT DEFAULTS", false);
    
    // Fit it
    exporterGUI->autoSizeToFitWidgets();
    
    // Add the event
    ofAddListener( exporterGUI->newGUIEvent, this, &VideoFXExporter::guiEvent );

    exporterGUI->loadSettings("GUI/exporter.xml");
}

void VideoFXExporter::guiEvent(ofxUIEventArgs &e) {

    if (e.widget->getName() == "EXPORT PAYLOAD" ) {
        ofxUIButton *button = (ofxUIButton *)e.widget;
        if (button->getValue()) {
            string payloadName = payloadNameTextInput->getTextString();
            string commands = commandTextInput->getTextString();
            saveToFile( payloadName, EXPORT_SELECTED, commands );
            return;
        }
        return;
    }
    
    if (e.widget->getName() == "EXPORT DEFAULTS" ) {
        ofxUIButton *button = (ofxUIButton *)e.widget;
        if (button->getValue()) {
            saveToFile( "defaults", EXPORT_DEFAULT );
            return;
        }
        return;
    }
    
    exporterGUI->saveSettings("GUI/exporter.xml");
}

#pragma mark - Saving

string VideoFXExporter::exportDirectory = "json/";

void VideoFXExporter::saveToFile(string payloadName, exportType type, string command) {
    Json::Value effects = getEffectsAsJson(payloadName, type);
    
    if (type != EXPORT_DEFAULT && command.length() > 0) {
        effects["command"] = command;
    }
    
    // JSON content
    string jsonStr = getJsonAsString( effects );
    // Write the file
    ofBuffer msg(jsonStr.c_str(), jsonStr.length());
    ofBufferToFile( ofToDataPath(exportDirectory + payloadName + ".json"), msg);
}

#pragma mark - JSON Utils

Json::Value VideoFXExporter::getEffectsAsJson(string payloadName, exportType type) {
        // Bail if no effects
		if (!videoFx) { return; }

        // Root json node
		Json::Value json;
    
        // Preset name
        json["payloadName"] = payloadName;
    
        // Array for each effect
        Json::Value effectsJson(Json::arrayValue);
        
		vector<BaseEffect*> effects = videoFx->effects;
		for ( int i=0; i<effects.size(); ++i) {
            Json::Value effectJson = getEffectAsJson(effects[i], type);
            if (effectJson["maps"].size() > 0) {
                effectsJson.append( effectJson );
            }
		}
    
        json["effects"] = effectsJson;

        // cout << json << endl;
        
        return json;
}

Json::Value VideoFXExporter::getEffectAsJson(BaseEffect *effect, exportType type) {
	// Set the name of the effect
    Json::Value effectJson;
    effectJson["name"] = effect->name;

    // Add the uniforms
    //Json::Value mapsJson(Json::arrayValue);
	ShaderMap map;
    if (type != EXPORT_DEFAULT) {
        map = effect->getShaderMap();
    } else {
        map = effect->getShaderDefaultMap();
    }
    
    if (type == EXPORT_ALL || type == EXPORT_DEFAULT) {
        effectJson["maps"] = map;
        return effectJson;
    }
    
    ShaderMap selectedMap;
    Json::Value::Members members = map.getMemberNames();
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) {
        if ( effectToggles[effect->name][*it]->getValue() ) {
            selectedMap[*it] = map[*it];
        }
    }
    effectJson["maps"] = selectedMap;
    return effectJson;

}

string VideoFXExporter::getJsonAsString(Json::Value json) {
    Json::StyledWriter writer;
    return writer.write( json );
}