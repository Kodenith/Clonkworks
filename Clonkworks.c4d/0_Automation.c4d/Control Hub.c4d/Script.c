/*-- Control Hub --*/

#strict 2

#include DOOR
#include CXEC
#include BAS9

local Conveyor;

func SoundOpenDoor(){
  Sound("SteelGate2");
}

func SoundCloseDoor(){
  Sound("SteelGate2");
}

func SoundConnect(){
  Sound("ConnectionSuccess");
}

func Initialize(){
    if(Conveyor = FindObject2(Find_AtPoint(0,30),Find_ID(CNVY),Find_NoContainer(),Find_Func("IsRotorTarget"))){
        LocalN("Extension",Conveyor) = this;
        SetGraphics("Funnel",this,GetID());
        Sound("Connect");
        if(basement) RemoveObject(basement);
    }
}

func RejectCollect(oId,pObj){
    if(!Conveyor) return(1);
    DebugLog("%v",AbsX(GetX(pObj)) );
    if(LocalN("Speed",Conveyor) > 5 && AbsX(GetX(pObj)) < -28) return(0);
    if(LocalN("Speed",Conveyor) < -5 && AbsX(GetX(pObj)) > 28) return(0);
    return(1);
}

func Collection(){
    Sound("Clonk");
}

func CheckConveyor(){
    if(Conveyor && OnFire(Conveyor)){
        //temporary
        RemoveObject();
    }
}