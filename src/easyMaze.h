#include "ofMain.h"
#include "common.h"
#include "ofxBiquadFilter.h"

class EasyMaze : public commonFunctions{
    
public:
    ofPolyline maze;
    int end,start;
    bool go = false;

    ofxBiquadFilter2f filter;
    ofPoint average;
    
    void setup(commonObjects*_co, vector<Button>*b){
        buttons = b;
        co = _co;
        svg.load("svg/03_EasyMaze.svg");
        polys = getPolyline(svg);
        maze = polys[0];
        
        start = 300;
        end = 1920-300;
        
        filter.setFc(0.05f);
        
    };
    
    bool isDone(){
        bool isInside=false;
        bool allAreDead=true;
        if(go && filter.value().x > end) isInside = true;
        
        if(isInside){
            for(int i = 0; i<buttons->size();i++){
                
                if(!buttons->at(i).on && buttons->at(i).isPlaying){
                    isInside=false;
                    break;
                }
                
                if(!buttons->at(i).isDead()){
                    allAreDead =false;
                }
            }
        }
        
        if(allAreDead)isInside=false;
        return isInside;
    };
    
    
    void update(){
        float x = 0.0;
        float y = 0.0;
        float indx = 0.0;
        if(!isDone()){
            for(int i=0; i<buttons->size(); i++) {
                buttons->at(i).update(co->attraction ); // the is on is checked in button!
                
                if(!buttons->at(i).isDead() && buttons->at(i).on){ // its on, not dead. (if it is not playing, it can not be on)
                    x+=buttons->at(i).getBiquadPos().x;
                    y+=buttons->at(i).getBiquadPos().y;
                    indx++;
                }
            }
            
            if(indx>0)average = ofPoint(x/indx , y/indx);
            filter.update(average);
        }
        if(go)
            if(!maze.inside(filter.value()))go = false;
        if(filter.value().x < start)go=true;
    };
    
    void draw(){
        
        ofSetColor(255);
        svg.draw();
        
        for(int i=0; i<buttons->size(); i++) {
            buttons->at(i).draw();
            if(co->debug){
                buttons->at(i).drawDebug();
            }
        }
        
        
        ofSetColor(255,100,100);
        ofPushMatrix();
        ofTranslate(filter.value());
        
        ofColor c;
        if(go)c.set(0,255,0);
        else c.set(255,0,0);
        
        float rad = 60.f;
        ofSetColor(c);
        // body
        c.setSaturation(180);
        ofDrawCircle(0,0,rad);
        
        //head
        c.setSaturation(230);
        ofSetColor(c);
        float eye = -rad + rad*0.45/2;
        ofDrawEllipse(0,eye, rad*0.85,rad*0.45);
    
        //stomach
        c.setSaturation(122);
        ofSetColor(c);
        ofDrawEllipse(0,rad*0.35,rad*1.5,rad*1.4);
        ofPopMatrix();
        
        //eyes
        //if(ofRandom(1000)>0.1){
        ofSetColor(0);
        ofDrawCircle(rad*0.3 ,eye, 10);
        ofDrawCircle(-rad*0.3,eye ,10);
        //}
        
    };
    
    
    void begin(){
        go = false;
        for(int i = 0; i<buttons->size();i++){
            buttons->at(i).setPosition(200, 1040);
        }
    };
    
    void reset(){
       
    };
    
    commonObjects * co;
    ofxSVG svg;
    vector<ofPolyline> polys;
    
    vector<Button>*buttons;


    
};