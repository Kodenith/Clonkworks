/*-- Neues Script --*/

#strict 2
#include CXEC

/* GENERAL AUTOMATED PRODUCTION TEMPLATE */

local Parent;
public func RejectConstruction(iX,iY,pBuilder){
	if(!ObjectOnConveyor(pBuilder)){
		Message("$TxtWrongPlacement$",pBuilder);
		pBuilder->Sound("Error");
		return(1);
	}
	
	return(0);
}

protected func Construction(){
	var Look;
	if(Look = FindObject2(Find_Func("IsConveyor"),Find_OnLine(0,0,0,10))){
		Parent = Look;
		LocalN("Extension",Parent) = this();
	}
	else RemoveObject();
	
	return(_inherited());
}

public func GetCDir(){
	if(!Parent) return(0);
	if(LocalN("Speed",Parent) == 0) return(0);
	if(LocalN("Speed",Parent) < 0) return(-1);
	return(1);
}

public func IsBuilt(){ return(GetCon() >= 100); }
protected func BuildNeedsMaterial(){ return(1); }

/* END */

func Update(){
	if(!IsBuilt()) return(0);
	if(!Parent){
		RemoveObject();
		return(0);
	}else if(OnFire(Parent)){
		Incinerate();
		return(0);
	}
	
	if(ActIdle()){
		if(GetCDir() != 0 && EnergyCheck(10000/2) && !FindObject2(Find_OnLine(-28,20,28,20), Find_NoContainer(),Find_OCF(OCF_CrewMember))) SetAction("Sawing");
	}else{
		if(GetCDir() == 0 || !EnergyCheck(10000/2)) SetAction("Idle");
	}
}

func CutUp(){
	var Objects = FindObjects(Find_OnLine(-28,20,28,20),Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Vehicle)),Find_NoContainer(),Find_Exclude(this()));
	
	if(!GetLength(Objects) || !Objects){
		Sound("chainsaw_loop",0,this(),50,0,-1,0,200);
		return(0);
	}else{
		Sound("chainsaw_loop",0,this(),50,0,+1,0,200);
	}
	
	for(var Obj in Objects){
		if(Obj->~IsTree()){
			Sound("Sawmill");
			CastParticles("Dust",RandomX(7,40),RandomX(20,40),RandomX(-28,28),18,10,20,RGBa(255,255,255),RGBa(255,255,255));
			DoDamage(RandomX(35,80),Obj);
			if(GetDamage(Obj) > (Obj->TreeStrength()*8)){
				Split2Components(Obj);
				continue;
			}
		}else if(GetCategory(Obj) & C4D_Vehicle){
			if(!Random(2)) Sound("AnvilWork*");
			if(!Random(6)) 	DoDamage(RandomX(10,30),Obj);
		}else if(GetCategory(Obj) & C4D_Living){
			if(!GetAlive(Obj)) continue;
			Sound("chainsaw_hit");
			if(GetMass(Obj) > 70) DoEnergy(-RandomX(5,20),Obj);
			else
			Punch(Obj,RandomX(20,30));
			if(!Random(6) && CrewMember(Obj)) Obj->Sound("Scream");
		}
	}
}