/*-- Neues Script --*/

#strict 2
#include RDEP

public func OreDepositType(){ return(GOLD); }
public func OreDepositMat(){ return("Gold"); }
func ColorMode(){ return(0); }
public func IsOreDeposit(){return(1);}

func MinOre(){ return(200); }
func MaxOre(){ return(400); }

func Coloration(){
	return(0);
}


func GetMined(pCaller,iNotDrillHead){
	if(Random(4)) return(0);
	Smoke(0,0,RandomX(5,20));
	
	if(!Random(2)) Sound("RockBreak*");
	else Sound("RockHit*");
	
	if(!Random(4)){
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
		if(!iNotDrillHead){
			SetPosition(GetX(pCaller),GetY(pCaller)+10,Ore);
		}else{
			SetPosition(GetX(Ore)+RandomX(-5,5),GetY(Ore)+RandomX(-5,5),Ore);
		  while(GBackSolid(AbsX(GetX(Ore)),AbsY(GetY(Ore))) || ObjectDistance(Ore,this()) <= 25){
			if(ObjectDistance(Ore,this()) > 25) break;
			var RotRef = Angle(GetX(Ore),GetY(Ore),GetX(),GetY());
			RotRef= (RotRef + 180) % 360 - 180;
			var iXDir = Sin(RotRef, 1);
			var iYDir = -Cos(RotRef, 1);
			SetX(GetX(Ore)-iXDir,Ore);
			SetY(GetY(Ore)-iYDir,Ore);
	     }
			
		}
	}else{
		return(1);
	}
}

func Damage(iChange){
	if(GetDamage() > 80 &&!OnFire() && !FindObject(GLDM) ){
		SetSolidMask();
		var targ = RandomX(3,8);
		for(var i = 0; i < targ; i++){
				Fling(CreateObject(OreDepositType()),RandomX(-6,6),RandomX(-6,6));
		}
		Explode(28);
		return(1);
	}
}