/*-- Neues Script --*/

#strict 2
#include BAS5

local Order;
local Progress;

//sign icon
local Sign;

//special
local Indestructible;
local OrderLocked;

//destruction and looks
public func SetCaveMode(iMode){
	if(GetAction() != "Idle" && GetAction() != "Walled") return(0);
	
	if(iMode) SetAction("Walled");
	else SetAction("Idle");
	return(1);
}

func Damage(iChange){
	if(iChange >= 23){
		CastParticles("FSpark", Random(9)+1,Random(39)+1, RandomX(-20,20),RandomX(-20,20), 50, 100, RGBa(255,255,255,128), RGBa(255,255,255,255));
		CastParticles("FSpark", Random(9)+1,Random(39)+1, RandomX(-20,20),RandomX(-20,20), 50, 200, RGBa(255,255,255,128), RGBa(255,255,255,255));
		if(!Random(5)){
			var Rock = CreateObject(RCK2);
			SetCon(RandomX(30,50),Rock);
			Fling(Rock,RandomX(-5,5),RandomX(0,-5));
		}
	}
	
	if(GetDamage() > 60){
		SetEntrance(0);
		if(Sign) RemoveObject(Sign);
		if(GetAction() == "Idle") SetAction("Crumble");
		else if(GetAction() == "Walled") SetAction("CrumbleWalled");
	}
}

private func Collapse(){ RemoveObject(); }

func Crumbling(){
	CastParticles("FSpark", Random(9)+1,Random(39)+1, RandomX(-30,30),GetDefBottom()-GetY(), 100, 200, RGBa(255,255,255,0), RGBa(255,255,255,255));
	if(!Random(5)){
			var Rock = CreateObject(RCK2);
			SetCon(RandomX(30,50),Rock);
			Fling(Rock,RandomX(-5,5),RandomX(0,-5));
	}else if(!Random(9)){
		var Rock = Contents(Random(ContentsCount(,this())),this());
		Exit(Rock);
		Fling(Rock,RandomX(-4,4),RandomX(0,-5));
	}
}



/* public func IsProducerOf (caller, def) {
  if (Order != def) return(0);
  if (!GetCon() == 100) return (0);
  return(1);
}

public func HowToProduce (clonk, def) {
	if(Order == def)
		DoMining(clonk);
	else
		return(0);
  return(1);
} */

public func ContainedDig(pClonk){
	[$TxtMine$|Image=Q_PK]
	DoMining(pClonk);
}
public func ContainedUp(pClonk){
	[$TxtOrder$|Image=LORY]
	if(GetEffectCount("QUARMining",pClonk)){
		RemoveEffect("QUARMining",pClonk);
	}
	if(!OrderLocked)
	ContextSetOrder(pClonk);
}
public func ContainedThrow(pClonk){
	if(GetEffectCount("QUARMining",pClonk)){
		RemoveEffect("QUARMining",pClonk);
		return(_inherited(pClonk));
	}
}
public func ContainedLeft(pClonk){
	if(GetEffectCount("QUARMining",pClonk)){
		RemoveEffect("QUARMining",pClonk);
		return(_inherited(pClonk));
	}
}
public func ContainedRight(pClonk){
	if(GetEffectCount("QUARMining",pClonk)){
		RemoveEffect("QUARMining",pClonk);
		return(_inherited(pClonk));
	}
}

func Initialize() {
  Order = ROCK;
  ResetProgress();
  SetEntrance(1);
  SetSign(Order);
  return(1);
}

public func CanMine(pClonk){
	if(GetEffectCount("QUARMining",pClonk)){
		RemoveEffect("QUARMining",pClonk);
		return(0);
	}
	
	if(Hostile(GetOwner(),GetOwner(pClonk))){
		pClonk->Sound("CommandFailure*");
		Message("$TxtFail3$",pClonk);
		return(0);
	}
	
	if(!GetPhysical("CanDig",0,pClonk)){
		pClonk->Sound("CommandFailure*");
		Message("$TxtFail1$",pClonk);
		return(0);
	}
	
	if(GetLength(FindObjects(Find_Container(this()),Find_Effect("QUARMining"))) > 4){
		pClonk->Sound("CommandFailure*");
		Message("$TxtFail2$",pClonk);
		return(0);
	}
	
	return(1);
}

public func IsOrderUnlocked(){ return(!OrderLocked); }

public func ContextSetOrder(pClonk){
	[$TxtOrder$|Image=LORY|Condition=IsOrderUnlocked]
	if(Hostile(GetOwner(),GetOwner(pClonk))){
		pClonk->Sound("CommandFailure*");
		Message("$TxtFail3$",pClonk);
		return(0);
	}
	
	CreateMenu(LORY,pClonk,this(),2,"$TxtOrder$");
	var m,j;
	while(m = MaterialName(j++)){
		if(GetMaterialVal("Blast2Object", "Material", Material(m))){
			var itm = GetMaterialVal("Blast2Object", "Material", Material(m));
			if(itm == GOLD && ObjectCount(GLDM)) continue;
			if(itm == ORE1 && ObjectCount(OREM)) continue;
			if(itm == ORE2 && ObjectCount(COPM)) continue;
			if(itm == ORE3 && ObjectCount(TITM)) continue;
			if(itm == ICE1) continue;
			AddMenuItem("$TxtOrder$: %s","SetOrder",itm,pClonk,,true);
		}
	}
}

public func SetOrder(itm,yell){
	Order = itm;
	ResetProgress();
	SetSign(itm);
	
	if(yell){
		Sound("Ding");
		Message("$TxtOrderSet$",this(),GetName(,Order));
	}
}

public func SetSign(item){
	if(!Sign){
		  Sign = CreateObject(QUAS);
		  Sign->SetAction("Attach",this());
		  Sign->SetActionData(256*0 + 8);
	}
	
	var Center = GetDefCoreVal("Offset", "DefCore", item,0);
	DebugLog("Offset: %d",Center);
	
	SetVertexXY(0,-Center,-Center,Sign);
	SetGraphics(0,Sign,item);
}

public func GetOrder(){
	return(Order);
}

public func ContextMine(pClonk){
	[$TxtMine$|Image=Q_PK]
	var iPlr = GetController(pClonk);
	var i,conk;
	while(conk = GetCursor(iPlr,i++)){
		DoMining(conk);
	}
}

public func DoMining(pClonk){
	if(!CanMine(pClonk)) return(0);
	if(Contained(pClonk) != this()){
		SetCommand(pClonk,"Enter",this());
		AppendCommand(pClonk,"Call",this(),pClonk,0,0,0,"DoMining");
	}else{
		AddEffect("QUARMining",pClonk,110,36,this(),,this());
	}
}

public func FxQUARMiningStart(pTarget, iEffectNumber, iTemp, targetQuarry){
	if(!targetQuarry) return(-1);
	EffectVar(0,pTarget,iEffectNumber) = targetQuarry;
	
	DebugLog("%v started mining in %v",pTarget,EffectVar(0,pTarget,iEffectNumber));
}

public func FxQUARMiningStop(object pTarget, int iEffectNumber, int iReason, bool fTemp){
	if(!EffectVar(0,pTarget,iEffectNumber)) return(0);
	DebugLog("%v stopped mining in %v",pTarget,EffectVar(0,pTarget,iEffectNumber));
}

//mining effect
public func FxQUARMiningTimer(object pTarget, int iEffectNumber, int iEffectTime){
	var Quarry = EffectVar(0,pTarget,iEffectNumber);
	
	if(Contained(pTarget) != Quarry){
		return(-1);
	}
	
	var ord,j,m,mat,shov;
	ord = LocalN("Order",Quarry);
	if(!ord) return(-1);
	while(m = MaterialName(j++)){
		if(GetMaterialVal("Blast2Object", "Material", Material(m)) == ord){
			mat = m;
			if(GetMaterialVal("Dig2Object", "Material", Material(m)) == ord){
				shov = true;
			}
		}
	}
	
	if(!mat) return(-1);
	var Power;
	if(shov) Power = GetPhysical("Dig",0,pTarget)/10000;
	else Power = 1;
	
	Power += pTarget->~MiningPower();
	
	//mining
	if(Power <= 0) return(-1);
	if(!GetPhysical("CanDig",0,pTarget)) return(-1);
	
	var soundType = "AnvilWork*";
	if(shov) soundType = "Dig*";
	
	Quarry->MineOrder(Power, soundType);
	pTarget->Sound(soundType);
	CastParticles("FSpark", Random(9)+1,Random(39)+1, RandomX(-10,10),RandomX(-10,10)+10, 25, 80, RGBa(255,255,255,128), RGBa(255,255,255,255));
}

func MineOrder(iPower,SoundType){
	Progress -= iPower;
	
	if(Progress <= 0){
		var Mined = CreateObject(Order);
		Enter(this(),Mined);
		Sound("RockBreak*");
		ResetProgress();
		
		for(var i in FindObjects(Find_ID(LORY),Find_Container(this()))){
			if(ContentsCount(,i) < i->MaxContents()){
				Enter(i,Mined);
				break;
			}else{
				SetCommand(i,"Exit");
			}
		}
	}
}

private func ResetProgress(){
	var ord,j,m,shov;
	ord = Order;
	if(!ord) return(-1);
	while(m = MaterialName(j++)){
		if(GetMaterialVal("Blast2Object", "Material", Material(m)) == ord){
			if(GetMaterialVal("Dig2Object", "Material", Material(m)) == ord){
				shov = true;
			}
		}
	}
	
	if(Order == GOLD) Progress = 350;
	else if(Order == CRYS) Progress = 450;
	else if(shov) Progress = 180;
	else Progress = 280;
}