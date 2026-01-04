/*-- Neues Script --*/

#strict 2
#include RDEP

public func OreDepositType(){ return(GOLD); }
public func OreDepositMat(){ return("Gold"); }
func ColorMode(){ return(0); }

func MinOre(){ return(25); }
func MaxOre(){ return(80); }

func Coloration(){
	return(0);
}


func GetMined(pCaller){
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
		SetPosition(GetX(pCaller),GetY(pCaller)+10,Ore);
	}else{
		return(1);
	}
}