/*-- Control Hub --*/

#strict 2

#include DOOR
#include CXEC
#include BAS9

local Conveyor;
local ObjectIndex;

func OnMenuSelection(Index,Clonk){
    if(GetMenu(Clonk) == CHUB){
        SetPlrView(GetController(Clonk),ObjectIndex[Index]);
        CreateParticle("Terashock",AbsX(GetX(ObjectIndex[Index])),AbsY(GetY(ObjectIndex[Index])),0,0,8*GetDefWidth(GetID(ObjectIndex[Index])),GetPlrColorDw(GetController(Clonk)));
    }
}

func SoundOpenDoor(){
  Sound("SteelGate2");
}

func SoundCloseDoor(){
  Sound("SteelGate2");
}

func SoundConnect(){
  Sound("ConnectionSuccess");
}

public func CanIO(){
    return(Conveyor != 0);
}

func isBuilt(){ return(GetCon() > 99); }

func Initialize(){
    ObjectIndex = [];
    if(Conveyor = FindObject2(Find_AtPoint(0,20),Find_ID(CNVY),Find_NoContainer(),Find_Func("IsRotorTarget"))){
        LocalN("Extension",Conveyor) = this;
        SetGraphics("Funnel",this,GetID());
        Sound("Connect");
        if(basement) RemoveObject(basement);
    }
}

func RejectCollect(oId,pObj){
    if(Contained(pObj)) return(0);
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

func ActivateEntrance(pObj){
    if(FindObject2(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_Container(this)) && (GetOCF(pObj) & OCF_CrewMember)){
        if(Contained(pObj) == this) return(_inherited(pObj));
        return(0);
    }
    return(_inherited(pObj));
}

// Connecting to other stations to give them orders.

public func ContextConnectToStation(pClonk){
    [$TxtConnect$|Image=CHBS|Condition=IsBuilt]
    if(Contained(pClonk) != this()){
        SetCommand(pClonk,"Enter",this);
        AppendCommand(pClonk,"Call",this,pClonk,0,0,0,"ContextConnectToStation");
        return(0);
    }

    ObjectIndex = FindObjects(Find_Distance(650),Find_Func("AutoOrderComp"),Find_NoContainer(),Find_Not(Find_Hostile(GetController(pClonk))));
    CreateMenu(CHBS,pClonk,this,0,"$TxtConnectFail2$",0,0,0,CHUB);
    for(var Obj in ObjectIndex){
        AddMenuItem(Format("$TxtMenuConnect$",GetName(Obj)),Format("CallStation(%v,%v)",ObjectNumber(Obj),ObjectNumber(pClonk)),GetID(Obj),pClonk);
    }
}

public func CallStation(pStation,pCaller){
    var Station = Object(pStation);
    var Caller = Object(pCaller);

    if(!Station || OnFire(Station)){
        Message("$TxtConnectFail3$",this);
        Sound("Discharge");
        return(0);
    }

    if(!EnergyCheck(10000)){
        Message("$TxtNoPower$",this);
        Sound("Discharge");
        return(0);
    }
    SetAction("Connected");
    Station->~DoConnectParticles(GetController(Caller));
    Station->~OrderMenu(Caller);
}