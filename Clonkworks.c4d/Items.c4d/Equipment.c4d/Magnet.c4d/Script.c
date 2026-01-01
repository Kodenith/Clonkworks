#strict 2

local MaterialCheck;
local iOn;

func Initialize() {
	iOn = false;
  MaterialCheck = [METL,GOLD,CRYS,CPIG,TTIG,ORE1,ORE2,ORE3]; //materials that can be attracted
  return(1);
}

func Attract(){
	if(!iOn) return(0);
	if(Contained()){
		if( !(GetCategory(ContainedTop(this())) & C4D_Vehicle) && !(GetCategory(ContainedTop(this())) & C4D_Living) ){
			return(0);
		}
	}
	
	if(!Random(1)){
		var me = this();
		if(Contained()) me = ContainedTop();
	CreateParticle("PSpark",RandomX(-10,10),RandomX(-10,10),0,0,RandomX(20,50),RGBa(255,RandomX(100,255),0,128),me);
	}
	
	var atrc = FindObjects(Find_Category(C4D_Object), Find_Distance(50), Find_Exclude(this()));
	var MaxAtr = 10;
	for(var metl in atrc){
		if(ObjectDistance(metl, this()) <= 10) continue;
		var canAttract = false;
		for(var mt in MaterialCheck){
			if(GetComponent(mt,,metl) != 0 || GetID(metl) == mt) canAttract = true;
		}
		if(!canAttract) continue;
		if(GetX(metl) > GetX(this())) SetXDir(GetXDir(metl)-RandomX(3,MaxAtr), metl);
		if(GetX(metl) < GetX(this())) SetXDir(GetXDir(metl)+RandomX(3,MaxAtr), metl);
		if(GetY(metl) > GetY(this())) SetYDir(GetYDir()-MaxAtr, metl);
		if(GetY(metl) < GetY(this())) SetYDir(GetYDir()+MaxAtr, metl);
	}
}

public func Activate(){
	[$Break$]
	 Sound("Click");
	 if(iOn){
		 Sound("Energy",0,this(),68,0,-1,0,40);
		 iOn = false;
	 }
	 else{
		 Sound("Energy",0,this(),68,0,+1,0,40);
		 iOn = true;
	 }
	 return(1);
}

protected func Hit()
{
  Sound("MetalHit*");
}

func IsMetal(){ return(true); }
func IsAnvilProduct(){ return(1); }

func Malfunction(){
	Activate();
}