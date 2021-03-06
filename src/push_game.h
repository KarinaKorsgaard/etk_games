#include "ofMain.h"
#include "common.h"



#include "ofMain.h"
#include "common.h"
#include "ofxBox2d.h"


class PushGame : public commonFunctions{
    
public:
    
   // vector<shared_ptr<ofxBox2dPolygon>> rects;
    vector<shared_ptr<ofxBox2dRect>> rects;
    vector <shared_ptr<ofxBox2dPolygon> > polyShapes;
    
    ofxBox2d world;
    vector<Button>*buttons;
    commonObjects * co;
    vector<ofPolyline>polys;
    ofxSVG svg;
    float deadZone;
    
    void setup(commonObjects*_co ){
        co = _co;
        
        world.init();
        world.setGravity(0, 1);
        world.createBounds(ofRectangle(0,0,1920*2, 1080));
        svg.load("svg/push_wall.svg");
        deadZone = 150;
    };
    
    bool isDone(){
        bool oneSurvivor = true;
        int count = 0;
        for(int i = 0 ; i< buttons->size();i++){
            if(buttons->at(i).isPlaying && !buttons->at(i).isDead())count++;
            if(count > 1){
                oneSurvivor = false;
                break;
            }
        }
        return oneSurvivor;
    };
    
    void update(){
    
        //if(beginTime<.2)beginTime += ofGetLastFrameTime();
        
        for(int i = 0 ; i< buttons->size();i++){
            Button * b = &buttons->at(i);
            if(!isDone() && !b->isDead()){
                
                if(i<buttons->size()/2 )b->update(co->attraction*b->getValue(),true, ofRectangle(deadZone+60,0,1980-deadZone,1080));
                else b->update(co->attraction*b->getValue()*2.,true, ofRectangle(1980,0,1980-deadZone,1080));
            }
            if(i<buttons->size()/2 && b->getBiquadPos().x+b->radius < deadZone){
                b->setValue(0);
                
            }
            else if(b->getBiquadPos().x-b->radius > 1920*2-deadZone){
                b->setValue(0);
                
            }
            if(b->isDead())b->box2Dcircle->setRadius(0);
           // b->drain(co->drainCoefficient1);
            b->drain(0);
            
        }
       // cout << rects[0]->getPosition() << endl;
        for(int i = 0; i<rects.size();i++){
            rects[i]->setVelocity(rects[i]->getVelocity()*0.1);
            rects[i]->addAttractionPoint(ofVec2f(1920,rects[i]->getPosition().y),co->blockForce);
        }

        world.update();
    }
    
    void draw(){
        ofSetColor(10,10,80);
        ofDrawRectangle(0,0,deadZone,1080);
        ofDrawRectangle(1920*2 - deadZone,0,deadZone,1080);
        
        ofSetColor(80,80,120);
        for(int i = 0 ; i< rects.size();i++)
            rects[i]->draw();
        
      //  svg.draw();
        
        for(int i = 0 ; i< buttons->size();i++){
            if(co->debug){
            
                if(i<buttons->size()/2 )buttons->at(i).drawDebug( ofRectangle(deadZone+60,0,1980-deadZone,1080));
                else buttons->at(i).drawDebug( ofRectangle(1980,0,1980-deadZone,1080));
            }
            buttons->at(i).draw(true,true);
        }
        
        
    };
    
    void begin(vector<Button>*b1){
        

        
        buttons = b1;
        
        
        for(int i = 0 ; i<buttons->size();i++){
            buttons->at(i).box2Dcircle->destroy();
            
            buttons->at(i).box2Dcircle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
            buttons->at(i).box2Dcircle.get()->setPhysics(CLAMP(buttons->at(i).getValue(),1,50), 0.0, 10.);
            buttons->at(i).box2Dcircle.get()->setup(world.getWorld(), 0, 0, 40);

        }


        createScene(getPolyline(svg, true));
        
        
        for(int i = 0; i<6 ; i++){
            
            shared_ptr<ofxBox2dRect> r = shared_ptr<ofxBox2dRect>(new ofxBox2dRect);
            r.get()->setPhysics(10., 0.1 , 50.);
            r.get()->setup(world.getWorld(), 1920 , 94 + 168*i + 10*i  , 1920 , 166);
            r.get()->alive = true;
            //r->setMassFromShape=false;
            //r->setVelocity(ofVec2f(0,0));
            rects.push_back(r);
            
//            ofPolyline newPoly;
//            newPoly.addVertex(ofVec2f(960      ,i*     1000./6. + i*10 ));
//            newPoly.addVertex(ofVec2f(960      ,(i+1)* 1000./6. + i*10 ));
//            newPoly.addVertex(ofVec2f(1920+960 ,i*     1000./6. + i*10 ));
//            newPoly.addVertex(ofVec2f(1920+960 ,(i+1)* 1000./6. + i*10 ));
//            
////            p.addVertex(ofVec2f(100* i,200));
////            p.addVertex(ofVec2f(100* i + 50,200));
////            
////            p.addVertex(ofVec2f(100* i,400));
////            p.addVertex(ofVec2f(100* i + 50,400));
////            
////            p.addVertex(ofVec2f(100* i,200));
//            
//            newPoly = newPoly.getResampledBySpacing(10);
//            newPoly.close();
//            
//            shared_ptr<ofxBox2dPolygon> poly = shared_ptr<ofxBox2dPolygon>(new ofxBox2dPolygon);
//            
//            poly->addVertices(newPoly.getVertices());
//            poly->setPhysics(6.0, 0.0, 5.0);
//          //  poly->triangulatePoly();
//            poly.get()->alive = true;
//            poly->create(world.getWorld());
//            rects.push_back(poly);

            

        }

        for(int i = 0 ; i< buttons->size();i++){
            if(buttons->at(i).isPlaying){
                if(i<buttons->size()/2)
                    buttons->at(i).setPosition(200,ofRandom(80,1000));
                else
                    buttons->at(i).setPosition( 1920*2 - 200 ,ofRandom(80,1000));
            }
//
           // buttons->at(i).box2Dcircle->setRadius(0);
        }

        beginTime= 0;
    };
    void reset(){
        if(polyShapes.size()>0){
            polyShapes.clear();
        }
        for(int i = 0; i<6 ; i++){
            rects[i]->destroy();
        }
        rects.clear();
        
    };
    
    void createScene(vector<ofPolyline>polys){
        
        if(polyShapes.size()==0){
            
            for(int i = 0 ;i <polys.size();i++){
                ofRectangle r = polys[i].getBoundingBox();
                if(polys[i].getVertices().size()>3 && (r.width<1900*2 && r.height < 1070)){
                    shared_ptr<ofxBox2dPolygon> poly = shared_ptr<ofxBox2dPolygon>(new ofxBox2dPolygon);
                    
                    poly->addVertices(polys[i].getVertices());
                    poly->setPhysics(0.0, 0.0, 0.0);
                    poly->triangulatePoly();
                    
                    poly->create(world.getWorld());
                    polyShapes.push_back(poly);
                }
            }
            
        }
    }


    float beginTime;
};

