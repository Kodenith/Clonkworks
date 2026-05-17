#include CLNK
#strict 2

local Filter;

func Initialize(){
    Filter = 50;
    return(_inherited());
}

protected func Detoxify(){
    if(Filter > 100 || FindObject(HZNF)) Filter = 100;
    if(Filter < 0) Filter = 0;
    if(!Filter) return(0);

    if(GBackLiquid() && FrameCounter()%28==0 && !FindObject(HZNF)){
        Filter-=50;
        if(Filter < 0) Filter = 0;
        CreateObject(HZMF)->SetAction("Broken");
    }

    if(ToxChance()){
        Filter--;
    }
}

protected func ToxChance(){
    var Count = ObjectCount2(Find_Distance(30),Find_ID(GS_1));
    if(!Count) return(0);
    if(Count <= 3) return(FrameCounter()%228==0);
    if(Count <= 9) return(FrameCounter()%152==0);
    if(Count <= 14) return(FrameCounter()%120==0);
    if(Count <= 20) return(FrameCounter()%72==0);
    if(Count >= 45) return(FrameCounter()%38==0);
    return(FrameCounter()%45==0);
}

public func InfobarTrigger(){
    if(!FindObject(HZNF))
	return(IB_Exist);
}

public func InfobarMax(){
	return(100);
}

public func InfobarValue(){
	return(Filter);
}

public func InfobarColor(){
    if(Filter > 80) return(RGBa(0,255,0));
    if(Filter > 60) return(RGBa(118,255,0));
    if(Filter > 40) return(RGBa(242,255,0));
    if(Filter > 20) return(RGBa(255,130,0));
    return(RGBa(255,0,0));
}

public func InfobarInfo(){
    return(Format("$TxtBar1$",Filter));
}

func GasResistant(){ if(Filter > 0) return(1); }

public func ControlDigDouble(pClonk){
    if(Contents(0)){
        var Cnt = Contents(0);
        if(GetOCF(Cnt) & OCF_Edible){
            if(Cnt->~IsPotion())
            Message("$TxtCantDrink$",this);
            else
            Message("$TxtCantEat$",this);
            Sound("CommandFailure1");
            return(1);
        }
    }
    return(_inherited(pClonk));
}

public func ControlSpecial(pClonk){
    [$TxtTakeOff$|Image=HZMS]
    if(GetEffectCount("IntMorphing",this)){
        if(GetAction() != "Walk"){
            Message("$TxtTakeOffError$",this,GetName(this));
            Sound("CommandFailure1");
            return(_inherited());
        }

        SetXDir(0);
        SetAction("PutOffArmor");
        return(1);
    }
}

protected func Undress(){
    var Suit = CreateObject(HZMS,0,0,GetOwner());
    if(!Contents()) Enter(this,Suit);
    LocalN("Filter",Suit)=Filter;
    MorphBack();
}