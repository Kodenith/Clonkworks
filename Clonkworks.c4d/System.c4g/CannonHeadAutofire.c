/*-- Autofire--*/

#strict 2
#appendto CTW2 //Combat cannon
#appendto CTW4 //Artilery cannon
#appendto CTW6 //antiair cannon
#appendto ACT2 //Flame cannon
#appendto ACT4 //Lightning cannon
#appendto LICA //liquid cannon

public func AutoFire(pByTower){
    if(!pByTower) pByTower = GetActionTarget();
    if(GetEffect("Reload",this())) return(0);

    //gunpowder based cannons first as theyre basically the same.
    if(GetID() == CTW2 || GetID() == CTW4){
        var FireMe = FindOtherContents(GUNP,pByTower);
        if(ObjectCount2(Find_ID(GUNP),Find_Container(pByTower)) < this()->CannonPowderNeeded(FireMe)) return(0);
        this()->Shoot(,FireMe);
    }

    //anti air cannon next
    if(GetID() == CTW6){
        if(!ObjectCount2(Find_Container(this()), Find_Or(Find_Func("IsArrow"),Find_Func("IsArrowPack")))){
            return(0);
        }

        this()->ComFire(,this());
    }

    //flame cannon next
    if(GetID() == ACT2){
        if(LocalN("FillLevel") < 50){
            if(FindContents(OBRL,pByTower)) this()->ComEnter(FindContents(OBRL,pByTower));
            else return(0);
        }

        this()->ComFire(pByTower);
    }

    //lightning cannon next
    if(GetID() == ACT4){
        if(!FindContents(CRYS)) return(0);
        this()->ComFire(pByTower);
    }

    //last, liquid cannon
    if(GetID() == LICA){
        if(FindContents(LBRL,pByTower)) this()->Shoot(LBRL);
        else if(FindContents(ABRL,pByTower)) this()->Shoot(ABRL);
        else if(FindContents(OBRL,pByTower)) this()->Shoot(OBRL);
        else if(FindContents(WBRL,pByTower)) this()->Shoot(WBRL);
        else return(0);
    }

    AddEffect("Reload",this,1,1,this);
    return(1);
}

public func AutoRange(){ 
    if(GetID() == ACT2) return(100);
    if(GetID() == ACT4) return(200);
    return(300);
}

public func DangerAhead(){
    var Found = FindObjects(Find_NoContainer(),Find_OnLine(Sin(GetR(), 13),-Cos(GetR(), 13),Sin(GetR(), AutoRange()),-Cos(GetR(), AutoRange())));
    for(suspect in Found){
       if( IsDangerous(suspect) ) return(1);
    }
}

public func IsDangerous(pObj){
    var pOwner = GetOwner(GetActionTarget());
    if(Hostile(pOwner,GetOwner(pObj)) && CrewMember(pObj)) return(1);
    else if(GetID(pObj) == MONS || GetID(pObj) == FMNS) return(1);
}

public func AutoReload(){
    if(GetID() == CTW6) return(38/2);
    if(GetID() == ACT2 || GetID() == ACT4) return(38);
    return(60);
}

func FxReloadTimer(pTarget,iEffectNum,Timer){
    if(Timer > AutoReload()) return(-1);
}