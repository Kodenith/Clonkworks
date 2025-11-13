/*-- Neues Script --*/

#strict 2
#include BAS5

local Orders;

func Initialize() {
  Orders = [];
  SetEntrance(1);
  return(1);
}

public func ContextMine(pClonk){
	[$TxtMine$]
	var iPlr = GetController(pClonk);
	var i,conk;
	while(conk = GetCursor(iPlr,i++)){
		DoMining(conk);
	}
}

public func ContextOrder(pClonk){
	[$TxtManageOrder$]
	if(Hostile(GetOwner(),GetOwner(pClonk))) return(0);
	var BannedID = [];
	//TODO: add icons for every context menu and menu.
	CreateMenu(,pClonk,this(),,"$TxtManageOrder$",,1);
	for(var i in Orders){
		if(InArray(i,BannedID) != -1) continue;
		AddMenuItem(GetName(,i),"DeincrementOrder",i,pClonk,ArrayDataCount(Orders,i),pClonk);
		ArrayAdd(BannedID,i);
	}
	
	AddMenuItem("$TxtAddOrder2$","MatSelection",,pClonk,,pClonk);
}

public func DeincrementOrder(item, pClonk){
	if(InArray(item,Orders) != -1) ArrayDeleteEntry(Orders,item);
	
	if(pClonk) ContextOrder(pClonk);
}

public func MatSelection(foo,pClonk){
	var j, m;
	CreateMenu(,pClonk,this(),,"foo");
	while(m = MaterialName(j++)){
		var i;
		if(i = GetMaterialVal("Blast2Object", "Material", Material(m))){
			if(i == GetMaterialVal("Dig2Object", "Material", Material(m))) continue;
			AddMenuItem("$TxtAdd$ %s","MatSelection2",i,pClonk,,pClonk);
		}
	}
}

public func MatSelection2(item,pClonk){
	CreateMenu(,pClonk,this(),,"foo");
	SetMenuSize (3, 1, pClonk); 
	AddMenuItem("$TxtAdd$ %s","Add1",item,pClonk,1,pClonk);
	AddMenuItem("$TxtAdd$ %s","Add5",item,pClonk,5,pClonk);
	AddMenuItem("$TxtAdd$ %s","Add10",item,pClonk,10,pClonk);
}

public func Add1(item,pClonk){
	var amount = 1;
	for(var i=0; i < amount; i++){
		ArrayAdd(Orders,item);
	}
	Message("$TxtAddOrder$",this(),amount);
	Sound("Ding");
	if(pClonk) ContextOrder(pClonk);
}

public func Add5(item,pClonk){
	var amount = 5;
	for(var i=0; i < amount; i++){
		ArrayAdd(Orders,item);
	}
	Message("$TxtAddOrder$",this(),amount);
	Sound("Ding");
	if(pClonk) ContextOrder(pClonk);
}

public func Add10(item,pClonk){
	var amount = 10;
	for(var i=0; i < amount; i++){
		ArrayAdd(Orders,item);
	}
	Message("$TxtAddOrder$",this(),amount);
	Sound("Ding");
	if(pClonk) ContextOrder(pClonk);
}


public func DoMining(pClonk){
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
}

//mining effect
public func FxQUARMiningTimer(object pTarget, int iEffectNumber, int iEffectTime){
	var Quarry = EffectVar(0,pTarget,iEffectNumber);
	
	if(Contained(pTarget) != Quarry){
		SetCommand(this(),"Exit");
		DebugLog("%v - Object left Quarry!",pTarget);
		return(-1);
	}
	
	if(GetLength(LocalN("Orders",Quarry)) == 0){
		SetCommand(this(),"Exit");
		DebugLog("%v - Quarry Mining Done!",pTarget);
		return(-1);
	}
	
	//var 1 is executing order
	if(!EffectVar(1,pTarget,iEffectNumber)) EffectVar(1,pTarget,iEffectNumber) = LocalN("Orders",Quarry)[0];
	var Order = EffectVar(1,pTarget,iEffectNumber);
	
	//basic creation of order
	var chance = 80;
	if(Order == GOLD) chance = 150;
	if(Order == CRYS) chance = 200;
	
	var clonkMulti = 10 * GetLength(FindObjects(Find_OCF(OCF_CrewMember), Find_Container(Quarry), Find_Effect("QUARMining")));
	chance -= clonkMulti;
	
	if(chance < 0) chance = 0;
	
	CreateParticle("PSpark",RandomX(-10,10),RandomX(-10,10)+10,0,0,RandomX(20,40),RGBa(255,255,0));
	pTarget->Sound("ClimbHit*");
	
	var ExploSize;
		//finding explosive
		for(var i in FindObjects(Find_Container(Quarry),Find_Category(C4D_Object))){
			if(GetDefCoreVal("Explosive", "DefCore", GetID(i))){
				ExploSize = i->~ExplodeSize();
				if(ExploSize <= 0){
					explo = 0;
				}else
				break;
			}
		}
		
	chance += ExploSize;
	
	if(!Random(chance)){
		
		//finding explosive
		var explo;
		for(var i in FindObjects(Find_Container(Quarry),Find_Category(C4D_Object))){
			if(GetDefCoreVal("Explosive", "DefCore", GetID(i))){
				explo = i;
				
				var ExploSize = explo->~ExplodeSize();
				if(ExploSize <= 0){
					explo = 0;
				}else
				break;
			}
		}
		
		var Mat, MatSize;
		var j;
		while(MaterialName(j++)){
			//DebugLog("%s: %v - %v",MaterialName(j),GetMaterialVal("Blast2Object", "Material", j),Order);
			if(GetMaterialVal("Blast2Object", "Material", j) == Order){
				Mat = j;
				MatSize = GetMaterialVal("Blast2ObjectRatio", "Material", Mat);
				break;
			}
		}
		
		if(!explo || !Mat){
			CreateContents(Order,Quarry,1);
			DeleteArrayItem(0,LocalN("Orders",Quarry));
			EffectVar(1,pTarget,iEffectNumber) = 0;
		
			pTarget->Sound("AnvilWork*");
			if(GetLength(Orders) == 0) MiningFinish();
		}else{
			var ExploSize = explo->~ExplodeSize();
			ShakeViewPort(ExploSize, 0, GetX(Quarry), GetY(Quarry));
			while(Random(3)){
				Smoke(RandomX(-5,5),RandomX(-10,4),RandomX(0,ExploSize));
			}
			if(!Random(3)){
				BlastObjects(RandomX(-ExploSize,ExploSize),RandomX(-ExploSize,ExploSize),ExploSize,Quarry,GetOwner(pTarget)+1);
			}
			ExploSize /= 6;
			if(!ExploSize) ExploSize = 1;
				
			var Amount = (ExploSize*MatSize)/MatSize;
			var RAmount = Amount;
			while(RAmount--){
				CreateContents(Order,Quarry,1);
				DeleteArrayItem(0,LocalN("Orders",Quarry));
				EffectVar(1,pTarget,iEffectNumber) = LocalN("Orders",Quarry)[0];
				Order = EffectVar(1,pTarget,iEffectNumber);
				if(!Order) break;
			}
			
			  var grade = BoundBy((ExploSize/10)-1,1,3);
			  Sound(Format("Blast%d", grade));
			  RemoveObject(explo);
			  
			  if(GetLength(Orders) == 0) MiningFinish();
		}
	}
}

public func MiningFinish(){
	Sound("ResearchDone");
	Message("$TxtComplete$",this());
}