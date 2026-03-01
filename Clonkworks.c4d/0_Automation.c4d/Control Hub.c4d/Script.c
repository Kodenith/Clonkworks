/*-- Control Hub --*/

#strict 2

#include DOOR
#include CXEC
#include BAS9

local Conveyor;
local ObjectIndex;
local AutoDepositTo;

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
    if(LocalN("Speed",Conveyor) > 5 && AbsX(GetX(pObj)) < -20) return(0);
    if(LocalN("Speed",Conveyor) < -5 && AbsX(GetX(pObj)) > 20) return(0);
    return(1);
}

func Collection(){
    Sound("Clonk");
}


func Update(){
    if(Conveyor && OnFire(Conveyor)){
        Incinerate();
    }

    if(Abs(FrameCounter()) % 19 == 0 && AutoDepositTo) DoAutoDeposit();
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

    DoEnergy(-10000);
    SetAction("Connected");
    Station->~DoConnectParticles(GetController(Caller));
    Station->~OrderMenu(Caller);
}

//Selecting Autodepositing
public func CanAutoDeposit(){
    return(!AutoDepositTo && Conveyor && isBuilt());
}

public func CanCancelAutoDeposit(){
    return(AutoDepositTo != 0 && Conveyor && isBuilt());
}

public func ContextBeginAutodep(pClonk){
    [$TxtAutodep$|Image=CHBS:1|Condition=CanAutoDeposit]
    if(Contained(pClonk) != this()){
        SetCommand(pClonk,"Enter",this);
        AppendCommand(pClonk,"Call",this,pClonk,0,0,0,"ContextBeginAutodep");
        return(0);
    }

    ObjectIndex = FindObjects(Find_Distance(650),Find_Func("AutoDepositHere"),Find_NoContainer(),Find_Not(Find_Hostile(GetController(pClonk))));
    CreateMenu(CHBS,pClonk,this,0,"$TxtConnectFail2$",0,0,0,CHUB);
    for(var Obj in ObjectIndex){
        AddMenuItem(Format("$TxtDepo$",GetName(Obj)),Format("SetDepositStation(%v,%v)",ObjectNumber(Obj),ObjectNumber(pClonk)),GetID(Obj),pClonk);
    }
}

func SetDepositStation(Objnum,ClonkNum){
    var pObj = Object(Objnum);
    var pClonk = Object(ClonkNum);

    Message("$TxtDepoSet$",this,GetName(pObj));
    SetAction("Connected");
    AutoDepositTo = pObj;
    SetPlrView(GetController(pClonk),this);
}

public func ContextStopAutodep(pClonk){
    [$TxtAutodepCancel$|Image=CHBS:2|Condition=CanCancelAutoDeposit]
    if(Contained(pClonk) != this()){
        SetCommand(pClonk,"Enter",this);
        AppendCommand(pClonk,"Call",this,pClonk,0,0,0,"ContextStopAutodep");
        return(0);
    }

    Message("$TxtDepoCancel$",this);
    Sound("Click");
    AutoDepositTo = 0;
}

func DoAutoDeposit(){
    var pID = AutoDepositTo->AD_NeedItem(this);
    if(!pID) return(0);
    
    var pItem = FindContents(pID);
    if(!pItem) return(0);

    if(Abs(LocalN("Speed",Conveyor)) < 5) return(0);
    if(!EnergyCheck(10000)) return(0);
    DoEnergy(-10000);
    var Right = LocalN("Speed",Conveyor)<0;
    if(!Right) Right = -1;

    Exit(pItem,-Right*40, GetDefBottom()-GetY());
    var UnstuckPixels = 6;
    while(Stuck(pItem) && UnstuckPixels--){
        SetY(GetY(pItem)-1,pItem);
    }

    return(1);
}

func Malfunction(){
    DoDamage(RandomX(10,30));
    if(GetDamage() >= 85) Incinerate();
}