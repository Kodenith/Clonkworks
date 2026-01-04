/*-- Neues Script --*/

#strict 2

public func IsOreDeposit(){return(1);}
public func OreDepositType(){ return(ROCK); }
public func OreDepositMat(){ return("Rock"); }
func ColorMode(){ return(0); }


func MinOre(){ return(10); }
func MaxOre(){ return(30); }

func Initialize() {
  SetAction("Rock");
  SetR(RandomX(-180,180));
  
  var phase;
  SetPhase(phase = Random(7));
  SetSolidMask(phase*48, 0, 48, 48,0,0,this());
  SetPicture(phase*48, 0, 48, 48,this());
  
  CalculateDepositContents();
  
  if(ColorMode() && OreDepositMat()){
	  var r,g,b,m;
	  m = RandomX(0,2);
	  
	  if(m == 0){
	  r = GetMaterialVal("Color","Material",Material(OreDepositMat()),0);
	  g = GetMaterialVal("Color","Material",Material(OreDepositMat()),1);
	  b = GetMaterialVal("Color","Material",Material(OreDepositMat()),2);
	  }
	  
	  if(m == 1){
	  r = GetMaterialVal("Color","Material",Material(OreDepositMat()),3);
	  g = GetMaterialVal("Color","Material",Material(OreDepositMat()),4);
	  b = GetMaterialVal("Color","Material",Material(OreDepositMat()),5);
	  }
	  
      if(m == 2){
	  r = GetMaterialVal("Color","Material",Material(OreDepositMat()),6);
	  g = GetMaterialVal("Color","Material",Material(OreDepositMat()),7);
	  b = GetMaterialVal("Color","Material",Material(OreDepositMat()),8);
	  }
	  
	  SetClrModulation(RGBa(r,g,b),this());
  }
  
  Coloration();
  
  return(1);
}

func Coloration(){
	var Dark = RandomX(155,255);
	SetClrModulation(RGBa(Dark,Dark,Dark));
}


//this func calculates the amount of desired ore in itself
func CalculateDepositContents(min,max){
	while(Contents(0)) RemoveObject(Contents(0)); //no old stuff
	var iCount = RandomX(MinOre(),MaxOre());
	CreateContents(OreDepositType(),this(),iCount);
	return(iCount);
}

func GetMined(pCaller){
	if(Random(4)) return(0);
	Smoke(0,0,RandomX(5,20));
	
	if(!Random(2)) Sound("RockBreak*");
	else Sound("RockHit*");
	
	if(!Random(8)){
		var Ore = Contents(0);
		if(!Ore){
			SetSolidMask();
			var targ = RandomX(3,7);
			for(var i = 0; i < targ; i++){
				Fling(CreateObject(OreDepositType()),RandomX(-6,6),RandomX(-6,6));
			}
			Explode(28);
			return(1);
		}
		Exit(Ore);
		SetPosition(GetX(pCaller),GetY(pCaller)+10,Ore);
	}else{
		return(1);
	}
}

//meteorite stuff
func Incineration(){
	SetCategory(C4D_Object);
	SetSolidMask();
	SetRDir(RandomX(-360,360));
}

func Hit(){
	if(!Random(3)) Sound("RockBreak*");
	if(OnFire()){
		if(!Random(10)){
			  SetR(RandomX(-180,180));
			  SetSolidMask(GetPhase()*48, 0, 48, 48,0,0,this());
			  SetCategory(1);
			  CreateObject(FLNT)->Explode(RandomX(40,60));
			  Extinguish();
			  MakeOre();
			  DoDamage(-999);
		}else{
			var x = Par(0)/10; var y =  Par(1)/10;
			CreateObject(FLNT)->Explode(RandomX(28,40));
			SetXDir(x); SetYDir(y);
		}
	}
}

func MakeOre(){
  var doof = true;
  CreateParticle("PSpark", RandomX(-3,3), RandomX(-5,5), RandomX(-2,2), RandomX(-2,2), RandomX(20000,30000), RGB(255,101,101));
CreateParticle("PSpark", RandomX(-3,3), RandomX(-5,5), RandomX(-2,2), RandomX(-2,2), RandomX(30000,50000), RGB(255,101,101));


for(var pItems in FindObjects(Find_Distance(150), Find_Category(C4D_Object), Find_NoContainer (),)){
 if(pItems) pItems->RemoveObject();
}


  for(var i=0; i<50; i++){
   if(!Random(12)){
   var pMat = CreateObject(QONT,RandomX(-120,120),RandomX(-120,120),-1);
   pMat->~Set(5,20,RandomX(-180,180), RandomX(-180,180), RandomX(-180,180),RandomX(8,20), 1, OreDepositMat(), 1);
   pMat->~CreateMyScape();
   }
   var pMat = CreateObject(QONT,RandomX(-70,70),RandomX(-70,70),-1);
   pMat->~Set(5,20,RandomX(-180,180), RandomX(-180,180), RandomX(-180,180),RandomX(8,20), 1, OreDepositMat(), 1);
   pMat->~CreateMyScape();


   }
   
   var am = Random(2);
   while(am > 0){
	   CreateObject(GetID(),RandomX(-70,70),RandomX(-70,70));
	   am--;
   }
}

func LaunchMete(){
	SetXDir(RandomX(-100,100));
	Incinerate();
	AddEffect("Meteo",this(),1,1,this());
}

func FxMeteoTimer(){
	if(GetCategory() & 1) return(-1);
	if(!OnFire() || Stuck()){
			SetR(RandomX(-180,180));
			SetSolidMask(GetPhase()*48, 0, 48, 48,0,0,this());
			SetCategory(1);
			CreateObject(FLNT)->Explode(RandomX(40,60));
			Extinguish();
			MakeOre();
			DoDamage(-999);
			return(-1);
	}
}

func Damage(iChange){
	if(GetDamage() > 200 && FindObject(RMND) && !OnFire()){
		SetSolidMask();
		var targ = RandomX(1,6);
		for(var i = 0; i < targ; i++){
				Fling(CreateObject(OreDepositType()),RandomX(-6,6),RandomX(-6,6));
		}
		Explode(28);
		return(1);
	}
}